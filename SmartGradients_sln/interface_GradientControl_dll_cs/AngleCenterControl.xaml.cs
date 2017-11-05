using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace GradientControls
{
    internal partial class AngleCenterControl : UserControl, ISelectable, IMovable
    {

        #region INTERNAL INTERFACE

        public AngleCenterControl()
        {
            InitializeComponent();
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

        #endregion

        private Boolean _Selected;
        private Boolean _Moving;
        private Point   _DragOffset;

        private void ApplySelectedStyle()
        {
            SelectionGroup.Visibility = Visibility.Visible;
        }

        private void RemoveSelectedStyle()
        {
            SelectionGroup.Visibility = Visibility.Hidden;
        }
    }
}
