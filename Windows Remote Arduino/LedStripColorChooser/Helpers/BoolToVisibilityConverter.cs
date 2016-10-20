using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml.Data;

namespace LedStripColorChooser.Helpers
{
	public class BoolToVisibilityConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, string language)
		{
			var invert = false;
			if (parameter != null)
				invert = Boolean.Parse(parameter.ToString());
			bool isVisible = (bool)value;

			if (invert)
				isVisible = !isVisible;

			return isVisible ? Windows.UI.Xaml.Visibility.Visible : Windows.UI.Xaml.Visibility.Collapsed;
		}

		public object ConvertBack(object value, Type targetType, object parameter, string language)
		{
			var invert = false;
			if (parameter != null)
				invert = Boolean.Parse(parameter.ToString());

			Windows.UI.Xaml.Visibility isVisible = (Windows.UI.Xaml.Visibility)value;

			if (invert)
				return isVisible != Windows.UI.Xaml.Visibility.Visible;
			return isVisible == Windows.UI.Xaml.Visibility.Visible;
		}
	}
}
