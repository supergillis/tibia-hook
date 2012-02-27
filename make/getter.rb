class Getter
  def get
    raise 'cannot call abstract method'
  end
end

class ChainedGetter < Getter
  def initialize(*getters)
    super()
    @getters = getters
  end

  def get
    temp = []
    @getters.each do |getter|
      temp = temp + getter.get
    end
    temp
  end
end

class FixedGetter < Getter
  def initialize(fixed)
    super()
    @fixed = fixed
  end

  def get
    @fixed
  end
end

class DirectoryGetter < Getter
  def initialize(*dirs)
    super()
    @dirs = dirs
  end

  def get
    temp = []
    @dirs.each do |dir|
      temp = temp + Dir[dir]
    end
    temp
  end
end
