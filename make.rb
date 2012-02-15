#!/usr/bin/ruby

require "optparse"
require_relative "runner"
require_relative "maker"

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
  "-I./lib/mologie-detours",
  "-I./src",
  "-I./src/linux",
  "-I./src/scripting",
  "-I./src/scripting/modules",
  "-I/usr/include/qt4",
  "-I/usr/include/qt4/QtCore",
  "-I/usr/include/qt4/QtScript"
]

def clean
  cleaner = DebugCommand.new("rm")

  DirectoryRunner.new("#{$OBJECTS_DIR}/**/*.o", cleaner).run
  DirectoryRunner.new("#{$MOC_DIR}/**/*.moc.cpp", cleaner).run
end

def all
  moccer = MoccerCommand.new($MOC)
  compiler = CompilerCommand.new($CC)
  linker = LinkerCommand.new($CC, $LIBRARY, $LDFLAGS)
 
  DirectoryRunner.new("#{$SOURCE_DIR}/**/*.h", moccer).run(*$INCLUDES)
  DirectoryRunner.new("**/*.cpp", compiler, ["lib/mologie-detours/hde32/src/hde32.cpp", "lib/mologie-detours/hde64/src/hde64.cpp"]).run(*($CFLAGS + $INCLUDES))

  objects = Dir["#{$OBJECTS_DIR}/**/*.o"]
  linker.run(*objects)
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
