
require "fiddle"
require "fiddle/import"

module GAMENGINE
  VERSION = "0.2.0"

  LIBRARY = ENV.fetch(
    "GAMENGINE_LIBRARY",
    File.expand_path(
      "../../native/build/libgamengine_core.so",
      __dir__
    )
  )

  module Native
    extend Fiddle::Importer

    dlload LIBRARY

    extern "void* ge_engine_create()"
    extern "void ge_engine_destroy(void*)"

    extern "unsigned long long ge_object_create(void*, const char*)"
    extern "unsigned long long ge_object_count(void*)"

    extern "int ge_object_remove(void*, unsigned long long)"

    extern "int ge_object_set_position(void*, unsigned long long, float, float, float)"
  end

  class Engine
    def initialize
      @handle = Native.ge_engine_create

      raise "Engine initialization failed" if @handle.to_i == 0
    end

    def create_object(name)
      id = Native.ge_object_create(@handle, name)

      raise "Object creation failed" if id == 0

      id
    end

    def count
      Native.ge_object_count(@handle)
    end

    def set_position(id, x, y, z)
      Native.ge_object_set_position(
        @handle, id, x, y, z
      ) == 1
    end

    def remove(id)
      Native.ge_object_remove(@handle, id) == 1
    end

    def close
      if @handle
        Native.ge_engine_destroy(@handle)
        @handle = nil
      end
    end
  end
end

engine = GAMENGINE::Engine.new

player = engine.create_object("Player")
engine.set_position(player, 5, 0, 3)

puts "GAMENGINE Ruby Bridge"
puts "Player ID: #{player}"
puts "Objects: #{engine.count}"

engine.close
