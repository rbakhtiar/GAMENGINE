
using System;
using System.Collections.Generic;
using System.Numerics;

namespace GAMENGINE
{
    public class GameObject
    {
        public string Name { get; set; }
        public Vector3 Position { get; set; }

        public GameObject(string name)
        {
            Name = name;
            Position = Vector3.Zero;
        }
    }

    public class Engine
    {
        public const string Version = "0.1.0";

        private readonly List<GameObject> objects = new();

        public GameObject CreateObject(string name)
        {
            var obj = new GameObject(name);
            objects.Add(obj);
            return obj;
        }

        public IReadOnlyList<GameObject> Objects => objects;

        public bool RemoveObject(GameObject obj)
        {
            return objects.Remove(obj);
        }
    }
}
