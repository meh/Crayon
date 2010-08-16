require 'mkmf'
require 'rake'
require 'rake/clean'

NAME    = 'Crayon'
VERSION = '0.0.1'

CC      = 'gcc'
CFLAGS  = "-Wall -Wextra -pedantic #{`nspr-config --cflags`.strip} #{`pcre-config --cflags`.strip} -Iinclude"
LDFLAGS = "#{`nspr-config --libs`.strip} #{`pcre-config --libs`.strip}"

SOURCES = FileList['sources/**/*.c']
OBJECTS = SOURCES.ext('o')

CLEAN.include(OBJECTS)
CLOBBER.include(NAME)

if ENV['DEBUG']
    CFLAGS << ' -g3 -DCRAYON_DEBUG'
else
    CFLAGS  << ' -Os'
    LDFLAGS << ' -s'
end

task :default => NAME

rule '.o' => '.c' do |t|
    sh "#{CC} #{CFLAGS} -o #{t.name} -c #{t.source}"
end

file NAME => OBJECTS do
    sh "#{CC} #{CFLAGS} #{OBJECTS} -o #{NAME} #{LDFLAGS}"
end
