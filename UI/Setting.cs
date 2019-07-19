using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace UI
{
    class Setting
    {
        public List<float> Volumes { get; private set; }
        public KeyGesture Hotkey { get; private set; }
        private int Index;

        public Setting()
        {
            Volumes = new List<float>();
            Index = 0;
        }
        public void AddVolume(float volume)
        {
            Volumes.Add(volume);
        }

        public void RemoveVolume(float volume)
        {
            Volumes.Remove(volume);
        }

        public void SetHotkey(KeyGesture key)
        {
            Hotkey = key;
        }

        public float Cycle()
        {
            Index++;
            return Volumes[Index];
        }
    }
}
