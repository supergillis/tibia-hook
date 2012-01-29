#!/usr/bin/ruby

require 'fileutils'

cc = "g++"
moc = "moc"

cflags="-fPIC -m32"
ldflags="-m32 -ldl -lpthread -L/usr/lib/i386-linux-gnu -lQtCore -lQtScript"
includes="-I. -I/usr/include/qt4 -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtScript"

Dir['**/*.o'].each do |file|
	print "cleaning ", file, "\n"
	FileUtils.rm file
end

Dir['**/*.moc.cpp'].each do |file|
	print "cleaning ", file, "\n"
	FileUtils.rm file
end

Dir['**/*.h'].each do |header|
	if File.readlines(header).grep(/Q_OBJECT/).length > 0
		print "moccing ", header, "\n"
		output = header[0..-3] + ".moc.cpp"
		result = `#{moc} #{includes} #{header} -o #{output}`
		status = $?.exitstatus
		exit unless status == 0
	end
end

Dir['**/*.cpp'].each do |source|
	print "compiling ", source, "\n"
	output = "obj/" + source[0..-5] + ".o"
	result = `#{cc} #{cflags} #{includes} #{source} -c -o #{output}`
	status = $?.exitstatus
	exit unless status == 0
end

output = "hook.so"
print "combining into ", output,"\n"
objects = Dir['**/*.o'].join(" ")
result = `#{cc} #{ldflags} #{objects} -shared -o #{output}`

status = $?.exitstatus
exit unless status == 0

print "done!\n"
