using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;


namespace GradientControls
{

    internal interface ISelectable
    {
        Boolean Selected { get; set; }
    }

    internal interface IMovable
    {
        Boolean Moving     { get; set; }
        Point   DragOffset { get; set; }
        void    Move(Rect Bounds, UIElement Parent, MouseEventArgs mouseMoveEventArgs);
    }

    public partial class AnchorPixelControl : UserControl, ISelectable, IMovable
    {

        #region INTERNAL INTERFACE

        public AnchorPixelControl()
        {
            InitializeComponent();
        }

        internal event EventHandler ColorChangeRequested;
        internal event EventHandler OpacityChangeRequested;
        internal event EventHandler DeletingRequested;

        public Color Color
        {
            get
            {
                return _Color;
            }
            set
            {
                _Color = value;
                Pixel.Fill = new SolidColorBrush(_Color);
                if (_Color.R + _Color.G + _Color.B < 255)
                {
                    Pixel.StrokeThickness = 1;
                }
                else
                {
                    Pixel.StrokeThickness = 0;
                }
            }
        }

        public Byte AnchPixOpacity
        {
            get
            {
                return _AnchPixOpacity;
            }
            set
            {
                _AnchPixOpacity = value;
                if (_AnchPixOpacity == 255)
                {
                    RemoveOpacityStyle();
                }
                else
                {
                    ApplyOpacityStyle();
                }
            }
        }

        Boolean ISelectable.Selected
        {
            get
            {
                return _Selected;
            }
            set
            {
                _Selected = value;
                if (_Selected)
                {
                    ApplySelectedStyle();
                }
                else
                {
                    RemoveSelectedStyle();
                }
            }
        }

        Boolean IMovable.Moving
        {
            get
            {
                return _Moving;
            }
            set
            {
                _Moving = value;
            }
        }

        Point IMovable.DragOffset
        {
            get
            {
                return _DragOffset;
            }
            set
            {
                _DragOffset = value;
            }
        }

        public void Move(Rect Bounds, UIElement Parent, MouseEventArgs mouseMoveEventArgs)
        {
            Int32 x, y;
            x = (Int32)(mouseMoveEventArgs.GetPosition(Parent).X - _DragOffset.X);
            y = (Int32)(mouseMoveEventArgs.GetPosition(Parent).Y - _DragOffset.Y);

            if      (x < Bounds.Left)  Canvas.SetLeft(this, Bounds.Left);
            else if (x > Bounds.Right) Canvas.SetLeft(this, Bounds.Right);
            else                       Canvas.SetLeft(this, x);


            if      (y < Bounds.Top)    Canvas.SetTop(this, Bounds.Top);
            else if (y > Bounds.Bottom) Canvas.SetTop(this, Bounds.Bottom);
            else                        Canvas.SetTop(this, y);
        }

        internal void Delete()
        {
            OnDeletingRequested();
        }

        internal void AllowOpacity()
        {
            SetOpacityItem.IsEnabled = true;
        }

        internal void ForbidOpacity()
        {
            SetOpacityItem.IsEnabled = false;
        }

        #endregion

        private Color   _Color          = Colors.Red;
        private Byte    _AnchPixOpacity = 255;
        private Boolean _Selected;
        private Boolean _Moving;
        private Point   _DragOffset;

        protected void OnDeletingRequested()
        {
            if (DeletingRequested != null)
            {
                DeletingRequested(this, EventArgs.Empty);
            }
        }

        protected void OnColorChangeRequested()
        {
            if (ColorChangeRequested != null)
            {
                ColorChangeRequested(this, EventArgs.Empty);
            }
        }

        protected void OnOpacityChangeRequested()
        {
            if (OpacityChangeRequested != null)
            {
                OpacityChangeRequested(this, EventArgs.Empty);
            }
        }

        private void ApplySelectedStyle()
        {
            SelectionGroup.Visibility = Visibility.Visible;
        }

        private void RemoveSelectedStyle()
        {
            SelectionGroup.Visibility = Visibility.Hidden;
        }

        private void ApplyOpacityStyle()
        {
            OpacityPolygon.Visibility = Visibility.Visible;
            Pixel.Opacity = 0.75;
        }

        private void RemoveOpacityStyle()
        {
            OpacityPolygon.Visibility = Visibility.Collapsed;
            Pixel.Opacity = 1;
        }   

        private void UserControl_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            OnColorChangeRequested();
        }

        private void DeleteItem_Click(object sender, RoutedEventArgs e)
        {
            Delete();
        }

        private void SetColorItem_Click(object sender, RoutedEventArgs e)
        {
            OnColorChangeRequested();
        }

        private void SetOpacityItem_Click(object sender, RoutedEventArgs e)
        {
            OnOpacityChangeRequested();
        }

        private void Polygon_MouseUp(object sender, MouseButtonEventArgs e)
        {
            OnOpacityChangeRequested();
        }
    }
}
