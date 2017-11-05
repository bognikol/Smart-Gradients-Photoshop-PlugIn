using System;
using System.Windows;
using System.Windows.Controls;


namespace GradientControls
{
    internal partial class MasterControl : UserControl
    {

        #region INTERNAL INTERFACE

        public MasterControl()
        {
            InitializeComponent();
            GradientState = new m_GradientState();
            AnchPixNum = 3;
            PxPySlider.ValueChanged += PxPySlider_ValueChanged;
            ParameterSlider.ValueChanged += ParameterSlider_ValueChanged;
        }

        internal event EventHandler GradientStateChanged;

        internal m_GradientState    GradientState;

        internal Int32 AnchPixNum
        {
            get
            {
                return _AnchPixNum;
            }
            set
            {
                _AnchPixNum = value;
                if (GradientState.Type == 0)
                {
                    if (GradientState.Parameter == m_GradientState.ParameterNotUsed)
                    {
                        if (_AnchPixNum > 6)
                        {
                            ParameterStck.IsEnabled = false;
                        }
                        else
                        {
                            ParameterStck.IsEnabled = true;
                        }
                    }
                    else
                    {
                        if (_AnchPixNum > 7)
                        {
                            ParameterStck.IsEnabled = false;
                        }
                        else
                        {
                            ParameterStck.IsEnabled = true;
                        }
                    }
                }
                else
                {
                    if (_AnchPixNum > 7 || _AnchPixNum < 3)
                    {
                        ComplexRadioButton.IsEnabled = false;
                    }
                    else
                    {
                        ComplexRadioButton.IsEnabled = true;
                    }
                }
                GradientState.ActiveAnchPixs = _AnchPixNum;
                _OnGradientStateSkipTriggerFlag = true;
                if (ParameterCheckBox.IsChecked == true)
                {
                    PxPySlider.Maximum = AnchPixNum - 1;
                }
                else
                {
                    PxPySlider.Maximum = AnchPixNum - 2;
                }
                UpdatePxPyTextBoxes();
                HarvestGradientState();
                _OnGradientStateSkipTriggerFlag = false;
            }
        }

        #endregion

        private Int32 _AnchPixNum = 3;
        private Boolean _OnGradientStateSkipTriggerFlag;

        protected void OnGradientStateChanged()
        {
            if (GradientStateChanged != null)
            {
                GradientStateChanged(this, EventArgs.Empty);
            }
        }

        private bool IsNumeric(String str)
        {
            Double numericOutput;
            return Double.TryParse(str, out numericOutput);
        }

        private void UpdatePxPyTextBoxes()
        {
            PxTextBox.Text = Convert.ToString(PxPySlider.Value);
            PyTextBox.Text = Convert.ToString(PxPySlider.Maximum + 1 - PxPySlider.Value);
        }

        private void LegacyRadioButton_Click(object sender, RoutedEventArgs e)
        {
            if (ComplexStck.Visibility == Visibility.Visible)
            {
                LegacyStck.Visibility  = Visibility.Visible;
                ComplexStck.Visibility = Visibility.Collapsed;
            }
            HarvestGradientState();
        }

        private void ComplexRadioButton_Click(object sender, RoutedEventArgs e)
        {
            if (LegacyStck.Visibility == Visibility.Visible)
            {
                ComplexStck.Visibility = Visibility.Visible;
                LegacyStck.Visibility  = Visibility.Collapsed;
            }
            HarvestGradientState();
        }

        private void PxPySlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!_OnGradientStateSkipTriggerFlag)
            {
                UpdatePxPyTextBoxes();
                HarvestGradientState();
            }
        }

        private void ParameterCheckBox_Click(object sender, RoutedEventArgs e)
        {
            _OnGradientStateSkipTriggerFlag = true;
            if (ParameterCheckBox.IsChecked == true)
            {
                ParameterTextBox.IsEnabled = true;
                ParameterSlider.IsEnabled  = true;
                PxPySlider.Maximum = AnchPixNum - 1;
            }
            else
            {
                ParameterTextBox.IsEnabled = false;
                ParameterSlider.IsEnabled  = false;
                PxPySlider.Maximum = AnchPixNum - 2;
            }
            UpdatePxPyTextBoxes();
            HarvestGradientState();
            _OnGradientStateSkipTriggerFlag = false;
        }

        private void ParameterSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            ParameterTextBox.Text = Convert.ToString(ParameterSlider.Value);
            HarvestGradientState();
        }

        private void ParameterTextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            if (IsNumeric(ParameterTextBox.Text))
            {
                if (Convert.ToInt32(ParameterTextBox.Text) > 400 || Convert.ToInt32(ParameterTextBox.Text) < 0)
                {
                    ParameterTextBox.Text = Convert.ToString(ParameterSlider.Value);
                }
                else
                {
                    ParameterSlider.Value = Convert.ToDouble(ParameterTextBox.Text);
                }
            }
            else
            {
                ParameterTextBox.Text = Convert.ToString(ParameterSlider.Value);
            }
        }

        private void DiffusionIndexComboBox_DropDownClosed(object sender, EventArgs e)
        {
            HarvestDiffusionIndex();
        }

        private void GraduationSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            HarvestGradientState();
            GraduationTextBox.Text = GraduationSlider.Value.ToString() + "%";
        }

        private void HarvestDiffusionIndex()
        {
            GradientState.DiffusionIndex = Convert.ToDouble(DiffusionIndexComboBox.Text);
            OnGradientStateChanged();
        }

        private void HarvestGradientState()
        {
            if (ComplexRadioButton.IsChecked == true)
            {
                GradientState.Type = 0;
                GradientState.Px = Convert.ToByte(PxTextBox.Text);
                GradientState.Py = Convert.ToByte(PyTextBox.Text);

                if (ParameterCheckBox.IsChecked == true)
                {
                    GradientState.Parameter = Convert.ToInt32(ParameterSlider.Value);
                }
                else
                {
                    GradientState.Parameter = m_GradientState.ParameterNotUsed;
                }
            }
            else if (LinearRadioButton.IsChecked == true)
            {
                GradientState.Type = 1;
                GradientState.Grad = GraduationSlider.Value / 100.0;
            }
            else if (RadialRadioButton.IsChecked == true)
            {
                GradientState.Type = 2;
                GradientState.Grad = GraduationSlider.Value / 100.0;
            }
            else
            {
                GradientState.Type = 3;
                GradientState.Grad = GraduationSlider.Value / 100.0;
            }

            OnGradientStateChanged();
        }  
    }
}
