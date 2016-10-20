using Microsoft.Maker.RemoteWiring;
using Microsoft.Maker.Serial;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace LedStripColorChooser
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class MainPage : Page, INotifyPropertyChanged
	{
		private const int BLUE_PIN = 6;
		private const int RED_PIN = 5;
		private const int GREEN_PIN = 3;

		public event PropertyChangedEventHandler PropertyChanged;

		private bool _IsConnected;

		public bool IsConnected
		{
			get { return _IsConnected; }
			set { _IsConnected = value; NotifyOfChange(nameof(IsConnected)); }
		}

		private int _Blue;
		public int Blue
		{
			get { return _Blue; }
			set { _Blue = value; NotifyOfChange(nameof(Blue)); }
		}

		private int _Green;
		public int Green
		{
			get { return _Green; }
			set { _Green = value; NotifyOfChange(nameof(Green)); }
		}

		private int _Red;
		public int Red
		{
			get { return _Red; }
			set { _Red = value;  NotifyOfChange(nameof(Red)); }
		}

		private IStream Connection { get; set; }
		private RemoteDevice Arduino { get; set; }

		public MainPage()
		{
			this.InitializeComponent();

			Connection = new BluetoothSerial("RN42-D924");
			Arduino = new RemoteDevice(Connection);

			DataContext = this;

			Connection.ConnectionEstablished += Connection_ConnectionEstablished;

			IsConnected = false;
			Connection.begin(115200, SerialConfig.SERIAL_8N1);
		}

		private void Connection_ConnectionEstablished()
		{
			Arduino.pinMode(RED_PIN, PinMode.PWM);
			Arduino.pinMode(BLUE_PIN, PinMode.PWM);
			Arduino.pinMode(GREEN_PIN, PinMode.PWM);

			var action = Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, new Windows.UI.Core.DispatchedHandler(() =>
			{
				IsConnected = true;
			}));
		}

		private void UpdateColor(object sender, EventArgs e)
		{
			var picker = sender as ColorPicker.ColorPicker;
			Arduino.analogWrite(RED_PIN, (ushort)picker.RedValue);
			Arduino.analogWrite(BLUE_PIN, (ushort)picker.BlueValue);
			Arduino.analogWrite(GREEN_PIN, (ushort)picker.GreenValue);
		}

		private void NotifyOfChange(string propertyName)
		{
			PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
		}
	}
}
