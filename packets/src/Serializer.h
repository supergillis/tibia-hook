#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QtGlobal>
#include <QList>

#include <stdexcept>

#include <PacketBuilder.h>
#include <PacketReader.h>
#include <Position.h>

#define ss_sequence(serializers...) s_sequence<serializers>
#define ss_repeat(count, serializer, member) s_repeat<count, serializer, decltype(member), &member>

#define ss_switch(member, cases...) s_switch<decltype(member), &member, cases>
#define ss_case(condition, serializer) s_case<condition, serializer>
#define ss_default(serializer) s_default<serializer>

#define ss_true s_true
#define ss_in(args...) s_in_quint8<args>

#define ss_nop s_nop
#define ss_exception s_exception
#define ss_message_exception(message) s_message_exception<message>
#define ss_skip(count) s_skip<count>
#define ss_member(member) s_member<decltype(member), &member>

template<typename ValueContainer>
static void print(ValueContainer value) {
    printf("value %d\n", value);
}

static void print(const QString& value) {
    printf("value %s\n", value.toStdString().c_str());
}

template<typename Self, typename Container>
struct s_typed {
    static void serialize(const Container& container, PacketBuilder& builder) {
        Self::structure::serialize(container, builder);
    }

    static void deserialize(Container& container, PacketReader& reader) {
        Self::structure::deserialize(container, reader);
    }

    template<typename...>
    struct s_sequence;

    template<typename Serializer, typename... Serializers>
    struct s_sequence<Serializer, Serializers...> {
        static void serialize(const Container& container, PacketBuilder& builder) {
            Serializer::serialize(container, builder);
            s_sequence<Serializers...>::serialize(container, builder);
        }

        static void deserialize(Container& container, PacketReader& reader) {
            Serializer::deserialize(container, reader);
            s_sequence<Serializers...>::deserialize(container, reader);
        }
    };

    template<typename Serializer>
    struct s_sequence<Serializer> {
        static void serialize(const Container& container, PacketBuilder& builder) {
            Serializer::serialize(container, builder);
        }

        static void deserialize(Container& container, PacketReader& reader) {
            Serializer::deserialize(container, reader);
        }
    };

    template<int count, typename Serializer, typename ContainerType, ContainerType Container::*member>
    struct s_repeat {
        static void serialize(const Container& container, PacketBuilder& builder) {
            Q_ASSERT((container.*member).size() >= count);
            for (int i = 0; i < count; ++i) {
                Serializer::serialize((container.*member)[i], builder);
            }
        }

        static void deserialize(Container& container, PacketReader& reader) {
            Q_ASSERT((container.*member).size() >= count);
            for (int i = 0; i < count; ++i) {
                Serializer::deserialize((container.*member)[i], reader);
            }
        }
    };

    template<typename AtomType, AtomType Container::*member, typename... Case>
    struct s_switch;

    template<typename AtomType, AtomType Container::*member, typename Case, typename... Cases>
    struct s_switch<AtomType, member, Case, Cases...> {
        static void serialize(const Container& container, PacketBuilder& builder) {
            if (Case::condition_type::template valid<AtomType, member>(container)) {
                Case::serializer_type::serialize(container, builder);
                return;
            }
            s_switch<AtomType, member, Cases...>::serialize(container, builder);
        }

        static void deserialize(Container& container, PacketReader& reader) {
            if (Case::condition_type::template valid<AtomType, member>(container)) {
                Case::serializer_type::deserialize(container, reader);
                return;
            }
            s_switch<AtomType, member, Cases...>::deserialize(container, reader);
        }
    };

    template<typename AtomType, AtomType Container::*member, typename Case>
    struct s_switch<AtomType, member, Case> {
        static void serialize(const Container& container, PacketBuilder& builder) {
            if (Case::condition_type::template valid<AtomType, member>(container)) {
                Case::serializer_type::serialize(container, builder);
            }
        }

        static void deserialize(Container& container, PacketReader& reader) {
            if (Case::condition_type::template valid<AtomType, member>(container)) {
                Case::serializer_type::deserialize(container, reader);
            }
        }
    };

    template<typename Condition, typename Serializer>
    struct s_case {
        typedef Condition condition_type;
        typedef Serializer serializer_type;
    };

    struct s_true;

    template<typename Serializer>
    struct s_default {
        typedef s_true condition_type;
        typedef Serializer serializer_type;
    };

    /**
      * Conditions
      */

    struct s_true {
        template<typename AtomType, AtomType Container::*member>
        static bool valid(const Container&) {
            return true;
        }
    };

    template <quint8...>
    struct s_in_quint8;

    template<quint8 value, quint8... values>
    struct s_in_quint8<value, values...> {
        template<typename AtomType, AtomType Container::*member>
        static bool valid(const Container& container) {
            if (value == container.*member) {
                return true;
            }
            return s_in_quint8<values...>::template valid<AtomType, member>(container);
        }
    };

    template<quint8 value>
    struct s_in_quint8<value> {
        template<typename AtomType, AtomType Container::*member>
        static bool valid(const Container& container) {
            if (value == container.*member) {
                return true;
            }
            return false;
        }
    };

    /**
      * Actions
      */

    struct s_nop {
        static void serialize(const Container&, PacketBuilder&) {
            // Do nothing
        }
        static void deserialize(Container&, PacketReader&) {
            // Do nothing
        }
    };

    struct s_exception {
        static void serialize(const Container&, PacketBuilder&) {
            throw std::runtime_error("unspecified message");
        }
        static void deserialize(Container&, PacketReader&) {
            throw std::runtime_error("unspecified message");
        }
    };

    template<const QString& message>
    struct s_message_exception {
        static void serialize(const Container&, PacketBuilder&) {
            throw std::runtime_error(message.toStdString().c_str());
        }
        static void deserialize(Container&, PacketReader&) {
            throw std::runtime_error(message.toStdString().c_str());
        }
    };

    template<quint16 count>
    struct s_skip {
        static void serialize(const Container&, PacketBuilder& builder) {
            builder.skip(count);
        }

        static void deserialize(Container&, PacketReader& reader) {
            reader.skip(count);
        }
    };

    template<typename AtomType, AtomType Container::*member>
    struct s_member;

    template<typename WriteContainer, WriteContainer (PacketReader::*read_func)(), void (PacketBuilder::*write_func)(WriteContainer), WriteContainer Container::*member>
    struct s_member_uint {
        static void serialize(const Container& container, PacketBuilder& builder) {
            (builder.*write_func)(container.*member);
        }

        static void deserialize(Container& container, PacketReader& reader) {
            container.*member = (reader.*read_func)();
        }
    };

    template<quint8 Container::*member>
    struct s_member<quint8, member>: public s_member_uint<quint8, &PacketReader::readU8, &PacketBuilder::writeU8, member> {};

    template<quint16 Container::*member>
    struct s_member<quint16, member>: public s_member_uint<quint16, &PacketReader::readU16, &PacketBuilder::writeU16, member> {};

    template<quint32 Container::*member>
    struct s_member<quint32, member>: public s_member_uint<quint32, &PacketReader::readU32, &PacketBuilder::writeU32, member> {};

    template<quint64 Container::*member>
    struct s_member<quint64, member>: public s_member_uint<quint64, &PacketReader::readU64, &PacketBuilder::writeU64, member> {};

    template<QString Container::*member>
    struct s_member<QString, member> {
        static void serialize(const Container& container, PacketBuilder& builder) {
            builder.writeString(container.*member);
        }

        static void deserialize(Container& container, PacketReader& reader) {
            QString value = reader.readString();
            container.*member = value;
        }
    };

    template<Position Container::*member>
    struct s_member<Position, member> {
        static void serialize(const Container& container, PacketBuilder& builder) {
            builder.writeU16((container.*member).x);
            builder.writeU16((container.*member).y);
            builder.writeU8((container.*member).z);
        }

        static void deserialize(Container& container, PacketReader& reader) {
            (container.*member).x = reader.readU16();
            (container.*member).y = reader.readU16();
            (container.*member).z = reader.readU8();
        }
    };
};

#endif
