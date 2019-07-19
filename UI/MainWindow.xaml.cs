using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Collections.ObjectModel;
using Interop;


namespace UI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private readonly ObservableCollection<Device> Devices;
        private static RoutedCommand Hotkeys;

        public MainWindow()
        {
            InitializeComponent();
            Devices = new ObservableCollection<Device>(Device.getDevices());
            CB_DEVICE_LIST.ItemsSource = Devices;
            CB_DEVICE_LIST.SelectedIndex = 0;
            Hotkeys = new RoutedCommand();
            InitSlider();
        }

        private void InitSlider()
        {
            int selectedIndex = CB_DEVICE_LIST.SelectedIndex;
            int currentVolume = (int)(Devices[selectedIndex].getVolume() * 100);
            VOLUME_SLIDER.Value = currentVolume;
            TB_VOLUME.Text = currentVolume.ToString();
        }

        private void VOLUME_SLIDER_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            int value = (int)e.NewValue;
            TB_VOLUME.Text = value.ToString();
        }

        private void CB_DEVICE_LIST_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            InitSlider();
        }

        private void RegisterHotkey()
        {
            
        }

        private void CycleVolume()
        {

        }
    }
}
