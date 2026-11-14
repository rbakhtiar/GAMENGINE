
module GAMENGINE
  VERSION = "0.1.0"

  class GameObject
    attr_accessor :name, :position

    def initialize(name)
      @name = name
      @position = [0.0, 0.0, 0.0]
    end
  end

  class Engine
    attr_reader :objects

    def initialize
      @objects = []
    end

    def create_object(name)
      object = GameObject.new(name)
      @objects << object
      object
    end

    def remove_object(object)
      @objects.delete(object)
    end
  end
end

engine = GAMENGINE::Engine.new
engine.create_object("Player")

puts "GAMENGINE #{GAMENGINE::VERSION}"
puts "Objects: #{engine.objects.length}"
