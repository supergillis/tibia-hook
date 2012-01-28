Class = null;

(function() {
	InstancePrototype = Object.create(Object.prototype);

	InstancePrototype.constructor = function() {
		// Do nothing by default
	};

	InstancePrototype.instanceOf = function(object) {
		var prototype;
		for (prototype = this.parent; prototype.parent; prototype = prototype.parent)
			if (prototype === object)
				return true;
		return false;
	};

	ClassPrototype = Object.create(Object.prototype);

	ClassPrototype.extend = function() {
		var prototype = this;
		var object = Object.create(prototype, {
			superclass: {
				value: prototype,
				enumerable: false,
				configurable: false,
				writable: false
			},
			instance: {
				value: Object.create(prototype.instance ? prototype.instance : InstancePrototype),
				enumerable: false,
				configurable: false,
				writable: false
			}
		});
		this.extended.apply(object, arguments);
		return object;
	};

	ClassPrototype.create = function() {
		var prototype = this.instance;
		var object = Object.create(prototype, {
			parent: {
				value: Object.getPrototypeOf(prototype),
				enumerable: false,
				configurable: false,
				writable: false
			},
			superclass: {
				value: this,
				enumerable: false,
				configurable: false,
				writable: false
			}
		});
		prototype.constructor.apply(object, arguments);
		return object;
	};

	ClassPrototype.extended = function() {
		// Do nothing by default
	};

	Class = Object.create(ClassPrototype);
})();
