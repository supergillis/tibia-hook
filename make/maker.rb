#!/usr/bin/ruby

require 'fileutils'

$MOC_BIN = "moc"
$CC_BIN = "g++"

class DebugCommand < Command
  def debug(program)
    print program
    print "\n"
  end
end

class MoccerCommand < DebugCommand
  def initialize(file_transformer, includes)
    super($MOC_BIN)
    @file_transformer = file_transformer
    @includes = includes
  end

  # Only accepts files that are more recently modified than the MOC files
  def accept(args)
    file = args.first
    output = @file_transformer.transform(file)
    if File.readlines(file).grep(/Q_OBJECT/).length > 0
      if File.exists?(output)
        return File.ctime(file) > File.ctime(output)
      end
      FileUtils.mkpath(File.dirname(output))
      return true
    end
    return false
  end

  def transform(args)
    file = args.shift
    output = @file_transformer.transform(file)
    @includes + args + [file, "-o", output]
  end
end

class CompilerCommand < DebugCommand
  def initialize(file_transformer, includes, cflags)
    super($CC_BIN)
    @file_transformer = file_transformer
    @includes = includes
    @cflags = cflags
  end

  # Only accepts files that are more recently modified than the object files
  def accept(args)
    file = args.first
    output = @file_transformer.transform(file)
    if File.exists?(output)
      return File.ctime(file) > File.ctime(output)
    end
    FileUtils.mkpath(File.dirname(output))
    return true
  end

  def transform(args)
    file = args.shift
    output = @file_transformer.transform(file)
    @includes + @cflags + args + [file, "-c", "-o", output]
  end
end

class LinkerCommand < DebugCommand
  def initialize(output, ldflags)
    super($CC_BIN)
    @output = output
    @ldflags = ldflags
  end

  # Only build if there are object files that are newer than the output file
  def accept(args)
    if not File.exists?(@output)
      return true
    end
    output_time = File.ctime(@output)
    args.each do |file|
      if File.ctime(file) > output_time
        return true
      end
    end
    return false
  end

  def transform(args)
    @ldflags + args + ["-shared", "-o", @output]
  end
end
