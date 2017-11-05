using System;
using System.Windows;
using System.Windows.Controls;


namespace GradientControls
{
    internal partial class ZoomControl : UserControl
    {

        #region INTERNAL INTERFACE

        public ZoomControl()
        {
            InitializeComponent();
            FitZoom = 1.0;
        }

        internal event EventHandler ZoomChanged;

        internal Double Zoom
        {
            get { return _Zoom; }
        }

        internal Double FitZoom 
        { 
            get
            {
                return _FitZoom;
            }
            set
            {
                _FitZoom    = value;
                _FitZoomIn  = _GetFitZoomInSelector();
                _FitZoomOut = _GetFitZoomOutSelector();
            }
        }

        internal void ZoomIn()  { _ZoomIn();  }
        internal void ZoomOut() { _ZoomOut(); }

        #endregion

        private Double _Zoom;
        private Double _FitZoom;
        private Int32  _FitZoomIn;
        private Int32  _FitZoomOut;
        
        protected void OnZoomChanged(Object sender, EventArgs e)
        {
            if (ZoomChanged != null)
            {
                ZoomChanged(sender, e);
            }
        }
        
        private void _SetZoom()
        {
            switch (ZoomComboBox.SelectedIndex)
            {
                case 0:
                    {
                        _Zoom = 0.0625;
                        break;
                    }
                case 1:
                    {
                        _Zoom = 0.125;
                        break;
                    }
                case 2:
                    {
                        _Zoom = 0.25;
                        break;
                    }
                case 3:
                    {
                        _Zoom = 0.50;
                        break;
                    }
                case 4:
                    {
                        _Zoom = 0.75;
                        break;
                    }
                case 6:
                    {
                        _Zoom = 1.0;
                        break;
                    }
                case 8:
                    {
                        _Zoom = 2.0;
                        break;
                    }
                case 9:
                    {
                        _Zoom = 3.0;
                        break;
                    }
                case 10:
                    {
                        _Zoom = 4.0;
                        break;
                    }
                case 12:
                    {
                        _Zoom = FitZoom;
                        break;
                    }
                default:
                    {
                        break;
                    }
            }

        }

        private Int32 _GetFitZoomInSelector()
        {
            if (_FitZoom < 0.75)
            {
                if (_FitZoom < 0.25)
                {
                    if (_FitZoom < 0.125)
                    {
                        if (_FitZoom < 0.0625)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 2;
                    }
                }
                else
                {
                    if (_FitZoom < 0.5)
                    {
                        return 3;
                    }
                    else
                    {
                        return 4;
                    }
                }
            }
            else
            {
                if (_FitZoom < 2.0)
                {
                    if (_FitZoom < 1.0)
                    {
                        return 6;
                    }
                    else
                    {
                        return 8;
                    }
                }
                else
                {
                    if (_FitZoom < 3.0)
                    {
                        return 9;
                    }
                    else
                    {
                        if (_FitZoom < 4.0)
                        {
                            return 10;
                        }
                        else
                        {
                            return 12;
                        }
                    }
                }
            }
        }

        private Int32 _GetFitZoomOutSelector()
        {
            if (FitZoom < 0.75)
            {
                if (FitZoom < 0.25)
                {
                    if (FitZoom < 0.125)
                    {
                        if (FitZoom < 0.0625)
                        {
                            return 12;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if (FitZoom < 0.5)
                    {
                        return 2;
                    }
                    else
                    {
                        return 3;
                    }
                }
            }
            else
            {
                if (FitZoom < 2.0)
                {
                    if (FitZoom < 1.0)
                    {
                        return 4;
                    }
                    else
                    {
                        return 6;
                    }
                }
                else
                {
                    if (FitZoom < 3.0)
                    {
                        return 8;
                    }
                    else
                    {
                        if (FitZoom < 4.0)
                        {
                            return 9;
                        }
                        else
                        {
                            return 10;
                        }
                    }
                }
            }
        }

        private void ZoomComboBox_Selected(object sender, RoutedEventArgs e)
        {
            _SetZoom();
            OnZoomChanged(this, EventArgs.Empty);
        }

        private void _ZoomIn()
        {
            switch (ZoomComboBox.SelectedIndex)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 8:
                case 9:
                    {
                        ZoomComboBox.SelectedIndex += 1;
                        break;
                    }

                case 4:
                case 6:
                    {
                        ZoomComboBox.SelectedIndex += 2;
                        break;
                    }
                case 11:
                    {

                        break;
                    }
                case 12:
                    {
                        ZoomComboBox.SelectedIndex = _FitZoomIn;
                        break;
                    }
                default:
                    break;

            }
        }

        private void _ZoomOut()
        {
            switch (ZoomComboBox.SelectedIndex)
            {
                case 1:
                case 2:
                case 3:
                case 4:
                case 9:
                case 10:
                    {
                        ZoomComboBox.SelectedIndex -= 1;
                        break;
                    }

                case 8:
                case 6:
                    {
                        ZoomComboBox.SelectedIndex -= 2;
                        break;
                    }
                case 12:
                    {
                        ZoomComboBox.SelectedIndex = _FitZoomOut;
                        break;
                    }
                default:
                    break;

            }
        }

        private void ZoomInButton_Click(object sender, RoutedEventArgs e)
        {
            _ZoomIn();
        }

        private void ZoomOutButton_Click(object sender, RoutedEventArgs e)
        {
            _ZoomOut();
        }
    }
}
