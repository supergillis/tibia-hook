class Transformer
  def transform(thing)
    thing
  end
end

class ChainedTransformer < Transformer
  def initialize(*transformers)
    super()
    @transformers = transformers
  end

  def transform(thing)
    temp = thing
    @transformers.each do |transformer|
      temp = transformer.transform(temp)
    end
    temp
  end
end

class PrefixPathJoinTransformer < Transformer
  def initialize(prefix)
    super()
    @prefix = prefix
  end

  def transform(thing)
    File.join(@prefix, thing)
  end
end

class ChangeSuffixTransformer < Transformer
  def initialize(old, new)
    super()
    @old = old
    @new = new
  end

  def transform(thing)
    thing.chomp(@old) + @new
  end
end
