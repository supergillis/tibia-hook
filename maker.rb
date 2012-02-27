#!/usr/bin/ruby

class DebugCommand < Command
  def debug(program)
    print program
    print "\n"
  end
end

class MoccerCommand < DebugCommand
  def moc_file(file)
    $MOC_DIR + file[$SOURCE_DIR.length .. -3] + ".moc.cpp"
  end

  # Only accepts files that are more recently modified than the MOC files
  def accept(*args, file)
    output = moc_file(file)
    lines = File.readlines(file)
    if lines.grep(/Q_OBJECT/).length > 0
      if File.exists?(output)
        return File.ctime(file) > File.ctime(output)
      end
      return true
    end
    false
  end

  def transform(*args, file)
    output = moc_file(file)
    args + [file, "-o", output]
  end
end

class CompilerCommand < DebugCommand
  def object_file(file)
    $OBJECTS_DIR + "/" + file[0 .. -5] + ".o"
  end

  # Only accepts files that are more recently modified than the object files
  def accept(*args, file)
    output = object_file(file)
    if File.exists?(output)
      return File.ctime(file) > File.ctime(output)
    end
    true
  end

  def transform(*args, file)
    output = object_file(file)
    args + [file, "-c", "-o", output]
  end
end

class LinkerCommand < DebugCommand
  def initialize(command, output, extra_args)
    super(command)
    @output = output
    @extra_args = extra_args
  end
  
  def transform(*args)
    @extra_args + args + ["-shared", "-o", @output]
  end

  # Only build if there are object files that are newer than the output file
  def accept(*args)
    if not File.exists?(@output)
      return true
    end
    output_time = File.ctime(@output)
    args.each do |file|
      if File.ctime(file) > output_time
        return true
      end
    end
    false
  end
end
