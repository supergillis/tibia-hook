class Runner
  def run(args = [])
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

  def accept(args)
    true
  end

  def transform(args)
    args
  end

  def run(args = [])
    if accept(args)
      arguments = transform(args).join(" ")
      program = "#{command} #{arguments}"
      result = `#{program}`
      debug(program)
      status = $?.exitstatus
      exit unless status == 0
    end
  end
end

class ListRunner < Runner
  def initialize(list, runner)
    @list = list
    @runner = runner
  end

  def run(args = [])
    @list.each do |file|
      @runner.run([file] + args)
    end
  end
end
