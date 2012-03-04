#!/usr/bin/ruby

require "optparse"

$: << File.dirname(__FILE__)
require "getter"
require "transformer"
require "runner"
require "maker"

$BIN_DIR = "bin"
$LIB_DIR = "lib"
$SOURCE_DIR = "src"
$MOC_DIR = "moc"
$OBJECTS_DIR = "obj"
$PLUGIN_DIR = "plugins"

$OUTPUT = "hook.so"
$PLUGIN_OUTPUT = "plugin.so"

$CFLAGS = [
  "-m32",
  "-fPIC"
]

$LDFLAGS = [
  "-m32",
  "-ldl",
  "-lpthread",
  "-lQtCore",
  "-lQtGui",
  "-lQtScript"
]

$INCLUDES = [
  "-I./lib/mologie-detours",
  "-I./lib/qt-json",
  "-I./include",
  "-I./src",
  "-I/usr/include/qt4",
  "-I/usr/include/qt4/QtCore",
  "-I/usr/include/qt4/QtGui",
  "-I/usr/include/qt4/QtScript"
]

def compile(header_getter, source_getter, object_getter, output)
  moccer = MoccerCommand.new(ChainedTransformer.new(PrefixPathJoinTransformer.new($MOC_DIR), ChangeSuffixTransformer.new("h", "moc.cpp")), $INCLUDES)
  compiler = CompilerCommand.new(ChainedTransformer.new(PrefixPathJoinTransformer.new($OBJECTS_DIR), ChangeSuffixTransformer.new("cpp", "o")), $INCLUDES, $CFLAGS)

  ListRunner.new(header_getter.get, moccer).run
  ListRunner.new(source_getter.get, compiler).run

  linker = LinkerCommand.new(output, $LDFLAGS)
  linker.run(object_getter.get)
end

def clean
  cleaner = DebugCommand.new("rm")

  ListRunner.new(Dir[File.join($OBJECTS_DIR, "**", "*.o")], cleaner).run
  ListRunner.new(Dir[File.join($MOC_DIR, "**", "*.moc.cpp")], cleaner).run
end

def core
  header_getter = DirectoryGetter.new(File.join($SOURCE_DIR, "**", "*.h"))
  source_getter = ChainedGetter.new(DirectoryGetter.new(File.join($SOURCE_DIR, "**", "*.cpp")), DirectoryGetter.new(File.join($MOC_DIR, $SOURCE_DIR, "**", "*.cpp")), FixedGetter.new(["lib/mologie-detours/hde32/src/hde32.cpp", "lib/qt-json/json.cpp"]))
  object_getter = ChainedGetter.new(DirectoryGetter.new(File.join($OBJECTS_DIR, $SOURCE_DIR, "**", "*.o")), DirectoryGetter.new(File.join($OBJECTS_DIR, $MOC_DIR, $SOURCE_DIR, "**", "*.o")), DirectoryGetter.new(File.join($OBJECTS_DIR, $LIB_DIR, "**", "*.o")))
  output = File.join($BIN_DIR, $OUTPUT)
  # Ensure the directory exists
  FileUtils.mkpath(File.dirname(output))
  # Compile the core
  compile(header_getter, source_getter, object_getter, output)
end

def plugins
  Dir[File.join($PLUGIN_DIR, "*")].each do |plugin_dir|
    header_getter = DirectoryGetter.new(File.join(plugin_dir, "**", "*.h"))
    source_getter = ChainedGetter.new(DirectoryGetter.new(File.join(plugin_dir, "**", "*.cpp")), DirectoryGetter.new(File.join($MOC_DIR, plugin_dir, "**", "*.cpp")))
    object_getter = ChainedGetter.new(DirectoryGetter.new(File.join($OBJECTS_DIR, plugin_dir, "**", "*.o")), DirectoryGetter.new(File.join($OBJECTS_DIR, $MOC_DIR, plugin_dir, "**", "*.o")))
    output = File.join($BIN_DIR, plugin_dir, $PLUGIN_OUTPUT)
    # Ensure the directory exists
    FileUtils.mkpath(File.dirname(output))
    # Compile the plugin
    compile(header_getter, source_getter, object_getter, output)
  end
end

OptionParser.new do |opts|
  opts.banner = "Usage: make.rb option"
  opts.on("--all", "Build everything.") do
    core
    plugins
  end
  opts.on("--core", "Build the core system.") do
    core
  end
  opts.on("--plugins", "Build the plugins.") do
    plugins
  end
  opts.on("--clean", "Clean all object files.") do
    clean
  end
end.parse!
