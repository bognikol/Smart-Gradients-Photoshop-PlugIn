using System;
using System.Windows;

namespace GradientControls
{

    internal partial class SetOpacityWindow : Window
    {

        #region INTERNAL INTERFACE

        public SetOpacityWindow()
        {
            InitializeComponent();
        }

        internal Byte AnchPixelOpacity
        {
            get
            {
                return _Opacity;
            }
        }

        internal void ShowDialog(Byte Opacity)
        {
            OpacitySlider.Value = Math.Round(Opacity / 255.0 * 100.0);

            if (OpacitySlider.Value == 0)
                OpacityTextBox.Text = "0%";

            ShowDialog();
        }

        #endregion

        private Byte    _Opacity;
        private Boolean _SuspendTextBoxUpdate;

        private String GetNumericFromOpacity(String str)
        {
            str = str.Trim();
            if (str.ToCharArray()[str.Length - 1].ToString() == "%")
            {
                return str.Substring(0, str.Length - 1);
            }
            return str;
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
        }

        private void OpacitySlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            _Opacity = (Byte)(OpacitySlider.Value / 100.0 * 255.0);
            if (_Opacity == 0)
            {
                OpacityTextBox.Text = "0%";
            }
            else
            {
                OpacityTextBox.Text = OpacitySlider.Value.ToString() + "%";
            }
        }

        private void OpacityTextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            Double numericOutput;
            if (Double.TryParse(GetNumericFromOpacity(OpacityTextBox.Text), out numericOutput))
            {
                if (!_SuspendTextBoxUpdate)
                {
                    _SuspendTextBoxUpdate = true;
                     OpacitySlider.Value  = (Int32)numericOutput;
                    _SuspendTextBoxUpdate = false;
                }
            }
            else
            {
                OpacityTextBox.Text = OpacitySlider.Value.ToString() + "%";
            }

        }
    }
}
