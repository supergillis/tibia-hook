class Runner
  def initialize
    if self.class == Runner
      raise 'cannot instantiate an abstract class'
    end
  end

  def run(*args)
    raise 'cannot call abstract method'
  end
end

class Command < Runner
  def initialize(command)
    @command = command
  end

  def command
    @command
  end

  def debug(program)
  end

  def accept(*args)
    true
  end

  def transform(*args)
    args
  end

  def run(*args)
    if accept(*args)
      arguments = transform(*args).join(" ")
      program = "#{command} #{arguments}"
      result = `#{program}`
      debug(program)
      status = $?.exitstatus
      exit unless status == 0
    end
  end
end

class DirectoryRunner < Runner
  def initialize(filter, runnable, ignores = [])
    @filter = filter
    @runnable = runnable
    @ignores = ignores
  end

  def run(*args)
    Dir[@filter].each do |file|
      if not @ignores.include? file
        @runnable.run(*(args + [file]))
      end
    end
  end
end
