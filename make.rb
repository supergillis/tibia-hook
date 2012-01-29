#!/usr/bin/ruby

require "optparse"
require_relative "runner"

$CC = "g++"
$MOC = "moc"

$SOURCE_DIR = "src"
$MOC_DIR = "moc"
$OBJECTS_DIR = "obj"

$LIBRARY = "hook.so"

$CFLAGS = [
  "-m32",
  "-fPIC"
]

$LDFLAGS = [
  "-m32",
  "-ldl",
  "-lpthread",
  "-lQtCore",
  "-lQtScript"
]

$INCLUDES = [
  "-I./src",
  "-I/usr/include/qt4",
  "-I/usr/include/qt4/QtCore",
  "-I/usr/include/qt4/QtScript"
]

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

# Only accepts files that more recently modified than the MOC files
  def accept(*args, file)
    output = moc_file(file)
    if File.readlines(file).grep(/Q_OBJECT/).length > 0
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

  # Only accepts files that more recently modified than the object files
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

def clean
  cleaner = DebugCommand.new("rm")

  DirectoryRunner.new("#{$OBJECTS_DIR}/**/*.o", cleaner).run
  DirectoryRunner.new("#{$MOC_DIR}/**/*.moc.cpp", cleaner).run
end

def all
  moccer = MoccerCommand.new($MOC)
  compiler = CompilerCommand.new($CC)
  linker = DebugCommand.new($CC)
 
  DirectoryRunner.new("#{$SOURCE_DIR}/**/*.h", moccer).run(*$INCLUDES)
  DirectoryRunner.new("**/*.cpp", compiler).run(*($CFLAGS + $INCLUDES))

  objects = Dir["#{$OBJECTS_DIR}/**/*.o"]
  arguments = $LDFLAGS + objects + ["-shared", "-o", $LIBRARY]
  linker.run(*arguments)
end

OptionParser.new do |opts|
  opts.banner = "Usage: make.rb option"
  opts.on("--clean", "Clean all object files") do
    clean
  end
  opts.on("--all", "Build everything") do
    all
  end
end.parse!
