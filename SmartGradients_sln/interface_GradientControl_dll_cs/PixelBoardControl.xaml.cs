using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;


namespace GradientControls
{
    internal partial class PixelBoardControl : UserControl
    {
        internal enum CursorMode
        {
            Arrow,
            Dragging
        }

        internal enum CoordinateContext
        {
            Image,
            Screen
        }

        #region INTERNAL INTERFACE

        public PixelBoardControl(m_GradientState gState, m_ProxyState pState, Point imageSize, Boolean opacityAllowed)
        {
            InitializeComponent();
            GradientState  = gState;
            ProxyState     = pState;
            ImageSize      = imageSize;
            OpacityAllowed = opacityAllowed;
            AnchorPixels   = new List<AnchorPixelControl>();
        }


        internal event EventHandler AnchorPixelRequestedColorChange;
        internal event EventHandler ProxyStateChanged;
        internal event EventHandler AnchorPixelChanged;
        internal event EventHandler NumberOfAnchorPixelsChanged;
        internal event EventHandler LegacyParametersChanged;


        internal m_GradientState    GradientState      { get; set; }
        internal m_ProxyState       ProxyState         { get; set; }
        internal Point              ImageSize          { get; set; }
        internal Boolean            OpacityAllowed     { get; set; }
        internal AnchorPixelControl CurrentAnchorPixel { get; set; }
        internal Rect               BoardRect;


        internal CursorMode CMode
        {
            get
            {
                return _CMode;
            }
            set
            {
                _CMode = value;
                if  (CMode == CursorMode.Dragging) Cursor = Cursors.Hand;
                else                               Cursor = Cursors.Arrow;
            }
        }

        internal Int32 ViewportLocationX
        {
            get
            {
                return ProxyState.ViewportLocationH;
            }
            set
            {
                ProxyState.ViewportLocationH = value;
            }
        }

        internal Int32 ViewportLocationY
        {
            get
            {
                return ProxyState.ViewportLocationV;
            }
            set
            {
                ProxyState.ViewportLocationV = value;
            }
        }

        internal Double ZoomFactor
        {
            get
            {
                return ProxyState.ZoomFactor;
            }
            set
            {

                ZoomFactorChanged(ProxyState.ZoomFactor, value);
                ProxyState.ZoomFactor = value;
                OnProxyStateChanged();
            }
        }

        internal void ShowAppropriateControls()
        {
            if (GradientState.Type == 1)
            {
                AngleVertex1.Visibility        = Visibility.Visible;
                AngleVertex2.Visibility        = Visibility.Visible;
                AngleCenterControl1.Visibility = Visibility.Collapsed;
            }
            else if (GradientState.Type == 3)
            {
                AngleVertex1.Visibility        = Visibility.Collapsed;
                AngleVertex2.Visibility        = Visibility.Collapsed;
                AngleCenterControl1.Visibility = Visibility.Visible;
            }
            else
            {
                AngleVertex1.Visibility        = Visibility.Collapsed;
                AngleVertex2.Visibility        = Visibility.Collapsed;
                AngleCenterControl1.Visibility = Visibility.Collapsed;
            }
        }

        internal void Redrag()
        {
            for (Int32 i = 0; i <= AnchorPixels.Count - 1; i++)
            {
                Canvas.SetTop (AnchorPixels[i], (Int32)(ZoomFactor * (GradientState.AnchPixs[i].Y - ViewportLocationY)) - Constants.AnchPixelControlDragOffset);
                Canvas.SetLeft(AnchorPixels[i], (Int32)(ZoomFactor * (GradientState.AnchPixs[i].X - ViewportLocationX)) - Constants.AnchPixelControlDragOffset);
            }

            Canvas.SetTop (AngleCenterControl1, (Int32)(ZoomFactor * (GradientState.CenterY - ViewportLocationY)) - Constants.AnchPixelControlDragOffset);
            Canvas.SetLeft(AngleCenterControl1, (Int32)(ZoomFactor * (GradientState.CenterX - ViewportLocationX)) - Constants.AnchPixelControlDragOffset);

            BoardRect.Location = new Point(((-Constants.WorkingSurfaceSize + ImageSize.X) / 2 - ViewportLocationX) * ZoomFactor - Constants.AnchPixelControlDragOffset, 
                                           ((-Constants.WorkingSurfaceSize + ImageSize.Y) / 2 - ViewportLocationY) * ZoomFactor - Constants.AnchPixelControlDragOffset);
            BoardRect.Size = new Size(Constants.WorkingSurfaceSize * ZoomFactor, Constants.WorkingSurfaceSize * ZoomFactor);

            OnProxyStateChanged();
        }

        #endregion


        private List<AnchorPixelControl> AnchorPixels;
        private CursorMode _CMode;

        protected void OnAnchorPixelRequestedColorChange()
        {
            if (AnchorPixelRequestedColorChange != null)
            {
                AnchorPixelRequestedColorChange(this, EventArgs.Empty);
            }
        }

        protected void OnProxyStateChanged()
        {
            if (ProxyStateChanged != null)
            {
                ProxyStateChanged(this, EventArgs.Empty);
            }
        }

        protected void OnAnchorPixelChanged()
        {
            if (AnchorPixelChanged != null)
            {
                AnchorPixelChanged(this, EventArgs.Empty);
            }
        }

        protected void OnNumberOfAnchorPixelsChanged()
        {
            if (NumberOfAnchorPixelsChanged != null)
            {
                NumberOfAnchorPixelsChanged(this, EventArgs.Empty);
            }
        }

        protected void OnLegacyParametersChanged()
        {
            if (LegacyParametersChanged != null)
            {
                LegacyParametersChanged(this, EventArgs.Empty);
            }
        }


        private void AddAnchorPixel(Int32 x, Int32 y, CoordinateContext CoordinatesContext, Color Color, Byte Opacity, Boolean Raise_NumberOfAnchorPixelsChanged_Event)
        {
            Boolean canAdd = false;

            if (GradientState.Type == 0)
            {
                if (GradientState.Parameter == m_GradientState.ParameterNotUsed)
                {
                    if (AnchorPixels.Count < 7)
                    {
                        canAdd = true;
                    }
                    else
                    {
                        MessageBox.Show("Cannot add anchor pixel!");
                    }
                }
                else if (GradientState.Parameter <= 400 && GradientState.Parameter >= 0)
                {
                    if (AnchorPixels.Count < 6)
                    {
                        canAdd = true;
                    }
                    else
                    {
                        MessageBox.Show("Cannot add anchor pixel!");
                    }
                }
            }
            else
            {
                if (AnchorPixels.Count < 25)
                {
                    canAdd = true;
                }
                else
                {
                    MessageBox.Show("Cannot add anchor pixel!");
                }
            }

            if (canAdd)
            {
                AnchorPixels.Add(new AnchorPixelControl());
                AnchorPixels.Last().Color = Color;

                if (OpacityAllowed)
                {
                    AnchorPixels.Last().AnchPixOpacity = Opacity;
                    AnchorPixels.Last().AllowOpacity();
                }
                else
                {
                    AnchorPixels.Last().AnchPixOpacity = 255;
                    AnchorPixels.Last().ForbidOpacity();
                }

                Container.Children.Add(AnchorPixels.Last());

                switch (CoordinatesContext)
                {
                    case CoordinateContext.Image:
                        {
                            Canvas.SetTop (AnchorPixels.Last(), ZoomFactor * (y - ViewportLocationY) - Constants.AnchPixelControlDragOffset);
                            Canvas.SetLeft(AnchorPixels.Last(), ZoomFactor * (x - ViewportLocationX) - Constants.AnchPixelControlDragOffset);
                        }
                        break;
                    case CoordinateContext.Screen:
                        {
                            Canvas.SetTop (AnchorPixels.Last(), y - Constants.AnchPixelControlDragOffset);
                            Canvas.SetLeft(AnchorPixels.Last(), x - Constants.AnchPixelControlDragOffset);
                        }
                        break;
                }

                AnchorPixels.Last().MouseDown               += AnchorPixelControl_MouseDown;
                AnchorPixels.Last().DeletingRequested       += AnchorPixelControl_DeletingRequested;
                AnchorPixels.Last().ColorChangeRequested    += AnchorPixelControl_ColorChangeRequested;
                AnchorPixels.Last().OpacityChangeRequested  += AnchorPixelControl_OpacityChangeRequested;

                UpdateAnchorPixelColor   (AnchorPixels.Last());
                UpdateAnchorPixelOpacity (AnchorPixels.Last());
                UpdateAnchorPixelLocation(AnchorPixels.Last());
                UpdateOpacityApplied();

                GradientState.ActiveAnchPixs = AnchorPixels.Count;

                if (Raise_NumberOfAnchorPixelsChanged_Event) OnNumberOfAnchorPixelsChanged();
            }
        }

        private void Container_Loaded(object sender, RoutedEventArgs e)
        {
            ProxyState.ViewportLocationH = (Int32)(ImageSize.X - ProxyState.ViewportSizeH / ProxyState.ZoomFactor) / 2;
            ProxyState.ViewportLocationV = (Int32)(ImageSize.Y - ProxyState.ViewportSizeV / ProxyState.ZoomFactor) / 2;

            AddAnchorPixel (50,                       50,                       CoordinateContext.Image, Colors.DarkOrange,  255, false);
            AddAnchorPixel((Int32)(ImageSize.X - 50), 50,                       CoordinateContext.Image, Colors.Plum,        255, false);
            AddAnchorPixel((Int32)(ImageSize.X / 2), (Int32)(ImageSize.Y - 50), CoordinateContext.Image, Colors.YellowGreen, 255, false);

            Canvas.SetTop (AngleCenterControl1, ProxyState.ViewportSizeV / 2);
            Canvas.SetLeft(AngleCenterControl1, ProxyState.ViewportSizeH / 2);

            Canvas.SetTop (AngleVertex1, ProxyState.ViewportSizeV - 100);
            Canvas.SetLeft(AngleVertex1, 100);

            Canvas.SetTop (AngleVertex2, 100);
            Canvas.SetLeft(AngleVertex2, ProxyState.ViewportSizeH - 100);

            GradientState.CenterX = (Int32)((Canvas.GetLeft(AngleCenterControl1) + Constants.AnchPixelControlDragOffset) / ZoomFactor + ViewportLocationX);
            GradientState.CenterY = (Int32)((Canvas.GetTop (AngleCenterControl1) + Constants.AnchPixelControlDragOffset) / ZoomFactor + ViewportLocationY);

            UpdateAllAnchorPixels();

            BoardRect.Location = new Point(((-Constants.WorkingSurfaceSize + ImageSize.X) / 2 - ViewportLocationX) * ZoomFactor - Constants.AnchPixelControlDragOffset, 
                                           ((-Constants.WorkingSurfaceSize + ImageSize.Y) / 2 - ViewportLocationY) * ZoomFactor - Constants.AnchPixelControlDragOffset);
            BoardRect.Size = new Size(Constants.WorkingSurfaceSize * ZoomFactor, Constants.WorkingSurfaceSize * ZoomFactor);

            OnProxyStateChanged();
        }

        private void ZoomFactorChanged(double oldZoom, double newZoom)
        {
            if (ProxyState.ViewportSizeH / newZoom > Constants.WorkingSurfaceSize)
            {
                ViewportLocationX = (Int32)(-ProxyState.ViewportSizeH / 2 / newZoom + ImageSize.X / 2);
            }
            else
            {
                ViewportLocationX = ViewportLocationX + (Int32)(ActualWidth * (1 / oldZoom - 1 / newZoom) / 2);
            }

            if (ProxyState.ViewportSizeV / newZoom > Constants.WorkingSurfaceSize)
            {
                ViewportLocationY = (Int32)(-ProxyState.ViewportSizeV / 2 / newZoom + ImageSize.Y / 2);
            }
            else
            {
                ViewportLocationY = ViewportLocationY + (Int32)(ActualHeight * (1 / oldZoom - 1 / newZoom) / 2);
            }

            for (Int32 i = 0; i <= AnchorPixels.Count - 1; i++)
            {
                Canvas.SetTop (AnchorPixels[i], (Int32)(newZoom * (GradientState.AnchPixs[i].Y - ViewportLocationY)) - Constants.AnchPixelControlDragOffset);
                Canvas.SetLeft(AnchorPixels[i], (Int32)(newZoom * (GradientState.AnchPixs[i].X - ViewportLocationX)) - Constants.AnchPixelControlDragOffset);
            }

            Canvas.SetTop (AngleCenterControl1, (Int32)(newZoom * (GradientState.CenterY - ViewportLocationY)) - Constants.AnchPixelControlDragOffset);
            Canvas.SetLeft(AngleCenterControl1, (Int32)(newZoom * (GradientState.CenterX - ViewportLocationX)) - Constants.AnchPixelControlDragOffset);

            BoardRect.Location = new Point(((-Constants.WorkingSurfaceSize + ImageSize.X) / 2 - ViewportLocationX) * newZoom - Constants.AnchPixelControlDragOffset, 
                                           ((-Constants.WorkingSurfaceSize + ImageSize.Y) / 2 - ViewportLocationY) * newZoom - Constants.AnchPixelControlDragOffset);
            BoardRect.Size = new Size(Constants.WorkingSurfaceSize * newZoom, Constants.WorkingSurfaceSize * newZoom);
        }

        private void UpdateAnchorPixelLocation(AnchorPixelControl aPix)
        {
            GradientState.AnchPixs[AnchorPixels.IndexOf(aPix)].X = (Int32)((Canvas.GetLeft(aPix) + Constants.AnchPixelControlDragOffset) / ZoomFactor + ViewportLocationX);
            GradientState.AnchPixs[AnchorPixels.IndexOf(aPix)].Y = (Int32)((Canvas.GetTop(aPix)  + Constants.AnchPixelControlDragOffset) / ZoomFactor + ViewportLocationY);
        }

        private void UpdateAnchorPixelColor(AnchorPixelControl aPix)
        {
            GradientState.AnchPixs[AnchorPixels.IndexOf(aPix)].CH0 = aPix.Color.R;
            GradientState.AnchPixs[AnchorPixels.IndexOf(aPix)].CH1 = aPix.Color.G;
            GradientState.AnchPixs[AnchorPixels.IndexOf(aPix)].CH2 = aPix.Color.B;
        }

        private void UpdateAnchorPixelOpacity(AnchorPixelControl aPix)
        {
            GradientState.AnchPixs[AnchorPixels.IndexOf(aPix)].CH3 = aPix.AnchPixOpacity;
        }

        private void UpdateAllAnchorPixels()
        {
            foreach (AnchorPixelControl pix in AnchorPixels)
            {
                UpdateAnchorPixelLocation(pix);
                UpdateAnchorPixelColor(pix);
                UpdateAnchorPixelOpacity(pix);
            }
        }

        private void UpdateAnchorPixelsAfterDeleted(Int32 Index)
        {
            for (Int32 i = Index; i <= AnchorPixels.Count - 1; i++)
            {
                UpdateAnchorPixelLocation(AnchorPixels[i]);
                UpdateAnchorPixelColor   (AnchorPixels[i]);
            }
        }

        private void UpdateAngleCenter()
        {
            GradientState.CenterX = (Int32)((Canvas.GetLeft(AngleCenterControl1) + Constants.AnchPixelControlDragOffset) / ZoomFactor + ViewportLocationX);
            GradientState.CenterY = (Int32)((Canvas.GetTop (AngleCenterControl1) + Constants.AnchPixelControlDragOffset) / ZoomFactor + ViewportLocationY);
        }

        private void UpdateLinearAngle()
        {
            Double x1 = Canvas.GetLeft(AngleVertex1) + Constants.AnchPixelControlDragOffset; Double x2 = Canvas.GetLeft(AngleVertex2) + Constants.AnchPixelControlDragOffset;
            Double y1 = Canvas.GetTop (AngleVertex1) + Constants.AnchPixelControlDragOffset; Double y2 = Canvas.GetTop (AngleVertex2) + Constants.AnchPixelControlDragOffset;
            Double dy = y1 - y2;
            Double dx = x1 - x2;
            if (dx == 0) dx = 0.00001;
            GradientState.Angle = -dy / dx;
        }

        private void UpdateOpacityApplied()
        {
            Boolean opacityApplied = false;

            foreach (AnchorPixelControl pix in AnchorPixels)
            {
                if (pix.AnchPixOpacity != 255)
                {
                    opacityApplied = true;
                    break;
                }
            }

            GradientState.OpacityApplied = opacityApplied;
        }

        private void AnchorPixelControl_ColorChangeRequested(object sender, EventArgs e)
        {
            CurrentAnchorPixel = (AnchorPixelControl)sender;
            OnAnchorPixelRequestedColorChange();
            UpdateAnchorPixelColor(CurrentAnchorPixel);

            OnAnchorPixelChanged();
        }

        private void AnchorPixelControl_OpacityChangeRequested(object sender, EventArgs e)
        {
            SetOpacityWindow window = new SetOpacityWindow();
            AnchorPixelControl pix = (AnchorPixelControl)sender;
            window.ShowDialog(pix.AnchPixOpacity);
            if ((bool)window.DialogResult)
            {
                pix.AnchPixOpacity = window.AnchPixelOpacity;
                UpdateAnchorPixelOpacity(pix);

                if (pix.AnchPixOpacity == 255)
                {
                    OnAnchorPixelChanged();
                    UpdateOpacityApplied();
                }
                else
                {
                    UpdateOpacityApplied();
                    OnAnchorPixelChanged();
                }
            }
        }

        private void AnchorPixelControl_DeletingRequested(object sender, EventArgs e)
        {
            CurrentAnchorPixel = (AnchorPixelControl)sender;
            Int32 i = AnchorPixels.IndexOf(CurrentAnchorPixel);

            Boolean canDelete = false;
            if (GradientState.Type == 0)
            {
                if (AnchorPixels.Count > 3)
                {
                    canDelete = true;
                }
                else
                {
                    MessageBox.Show("Cannot delete anchor pixel!");
                }
            }
            else if (GradientState.Type != 2)
            {
                if (AnchorPixels.Count > 2)
                {
                    canDelete = true;
                }
                else
                {
                    MessageBox.Show("Cannot delete anchor pixel!");
                }
            }
            else
            {
                if (GradientState.CentralPix != i && AnchorPixels.Count > 2)
                {
                    canDelete = true;
                }
                else
                {
                    MessageBox.Show("Cannot delete anchor pixel!");
                }
            }

            if (canDelete)
            {
                Container.Children.Remove(CurrentAnchorPixel);
                AnchorPixels.Remove(CurrentAnchorPixel);
                UpdateAnchorPixelsAfterDeleted(i);
                GradientState.ActiveAnchPixs = AnchorPixels.Count;
                OnNumberOfAnchorPixelsChanged();
            }
        }



        private Boolean MouseDownOverAnchPixFlag;
        private Boolean MouseDownOverBoardFlag;
        private Point BoardDragOffset;
        private List<Point> p = new List<Point>();
        private Int32 vlX;
        private Int32 vlY;
        private Int32 acX;
        private Int32 acY;
        private Boolean Selecting;
        private Boolean MovingSelected;
        private List<IMovable> ls = new List<IMovable>();

        private void UnselectAll()
        {
            foreach (AnchorPixelControl pix in AnchorPixels)
            {
                ((ISelectable)pix).Selected = false;
            }
            ((ISelectable)AngleCenterControl1).Selected = false;
            ((ISelectable)AngleVertex1).Selected = false;
            ((ISelectable)AngleVertex2).Selected = false;
        }

        private void PrepeareSelected(MouseEventArgs e)
        {
            ls.Clear();
            foreach (AnchorPixelControl pix in AnchorPixels)
            {
                if (((ISelectable)pix).Selected)
                {
                    ((IMovable)pix).DragOffset = new Point(e.GetPosition(pix).X, e.GetPosition(pix).Y);
                    ((IMovable)pix).Moving = true;
                    ls.Add(pix);
                }
            }
            if (((ISelectable)AngleCenterControl1).Selected)
            {
                ((IMovable)AngleCenterControl1).DragOffset = new Point(e.GetPosition(AngleCenterControl1).X, e.GetPosition(AngleCenterControl1).Y);
                ((IMovable)AngleCenterControl1).Moving = true;
                ls.Add(AngleCenterControl1);
            }
        }



        private void AngleVertex_MouseDown(object sender, MouseButtonEventArgs e)
        {

            var selectable   = sender as ISelectable;
            var movable      = sender as IMovable;
            var inputElement = sender as IInputElement;

            if (selectable == null || movable == null || inputElement == null) return;

            if (CMode == CursorMode.Arrow && ((Keyboard.GetKeyStates(Key.LeftShift)  != KeyStates.None && Keyboard.GetKeyStates(Key.LeftShift)  != KeyStates.Toggled) ||
                                              (Keyboard.GetKeyStates(Key.RightShift) != KeyStates.None && Keyboard.GetKeyStates(Key.RightShift) != KeyStates.Toggled)))
            {
                selectable.Selected = !selectable.Selected;
                Selecting = true;
            }
            else if (CMode == CursorMode.Arrow && e.LeftButton == MouseButtonState.Pressed)
            {
                if (selectable.Selected)
                {
                    Selecting = true;
                }
                else
                {
                    movable.DragOffset = new Point(e.GetPosition(inputElement).X, e.GetPosition(inputElement).Y);
                    movable.Moving = true;
                }
            }
        }

        private void AngleCenterControl_MouseDown(object sender, MouseButtonEventArgs e)
        {
            var selectable   = sender as ISelectable;
            var movable      = sender as IMovable;
            var inputElement = sender as IInputElement;

            if (selectable == null || movable == null || inputElement == null) return;

            if (CMode == CursorMode.Arrow && ((Keyboard.GetKeyStates(Key.LeftShift)  != KeyStates.None && Keyboard.GetKeyStates(Key.LeftShift)  != KeyStates.Toggled) ||
                                              (Keyboard.GetKeyStates(Key.RightShift) != KeyStates.None && Keyboard.GetKeyStates(Key.RightShift) != KeyStates.Toggled)))
            {
                selectable.Selected = !selectable.Selected;
                Selecting = true;
            }
            else if (CMode == CursorMode.Arrow && e.LeftButton == MouseButtonState.Pressed)
            {
                if (selectable.Selected)
                {
                    Selecting = true;
                    MovingSelected = true;
                    PrepeareSelected(e);
                }
                else
                {
                    MovingSelected = false;
                    movable.DragOffset = new Point(e.GetPosition(inputElement).X, e.GetPosition(inputElement).Y);
                    movable.Moving = true;
                }

            }
        }

        private void AnchorPixelControl_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (CMode == CursorMode.Arrow && ((Keyboard.GetKeyStates(Key.LeftShift)  != KeyStates.None && Keyboard.GetKeyStates(Key.LeftShift)  != KeyStates.Toggled) ||
                                              (Keyboard.GetKeyStates(Key.RightShift) != KeyStates.None && Keyboard.GetKeyStates(Key.RightShift) != KeyStates.Toggled)))
            {
                ((ISelectable)sender).Selected = !((ISelectable)sender).Selected;
                Selecting = true;
            }
            else if (CMode == CursorMode.Arrow && e.LeftButton == MouseButtonState.Pressed)
            {
                CurrentAnchorPixel = (AnchorPixelControl)sender;
                if (((ISelectable)CurrentAnchorPixel).Selected)
                {
                    Selecting = true;
                    MovingSelected = true;
                    PrepeareSelected(e);
                }
                else
                {
                    MovingSelected = false;
                    ((IMovable)CurrentAnchorPixel).DragOffset = new Point(e.GetPosition(CurrentAnchorPixel).X, e.GetPosition(CurrentAnchorPixel).Y);
                    MouseDownOverAnchPixFlag = true;
                }
            }
        }

        private void Container_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (!Selecting)
            {
                UnselectAll();
            }
            Selecting = false;
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                BoardDragOffset.X = e.GetPosition(Container).X;
                BoardDragOffset.Y = e.GetPosition(Container).Y;
                acX = (Int32)Canvas.GetLeft(AngleCenterControl1);
                acY = (Int32)Canvas.GetTop(AngleCenterControl1);
                vlX = ViewportLocationX;
                vlY = ViewportLocationY;
                p.Clear();
                for (Int32 i = 0; i <= AnchorPixels.Count - 1; i++)
                {
                    p.Add(new Point(Canvas.GetLeft(AnchorPixels[i]), Canvas.GetTop(AnchorPixels[i])));
                }
                MouseDownOverBoardFlag = true;
            }
            e.Handled = true;
        }

        private void Container_MouseUp(object sender, MouseButtonEventArgs e)
        {
            MouseDownOverAnchPixFlag = false;
            MouseDownOverBoardFlag = false;
            ((IMovable)AngleCenterControl1).Moving = false;
            ((IMovable)AngleVertex1).Moving = false;
            ((IMovable)AngleVertex2).Moving = false;
            MovingSelected = false;
        }

        private void Container_MouseEnter(object sender, MouseEventArgs e)
        {
            if (Mouse.LeftButton != MouseButtonState.Pressed)
            {
                MouseDownOverAnchPixFlag = false;
                ((IMovable)AngleCenterControl1).Moving = false;
                ((IMovable)AngleVertex1).Moving = false;
                ((IMovable)AngleVertex2).Moving = false;
                MovingSelected = false;
            }
            e.Handled = true;
        }

        private void Container_MouseLeave(object sender, MouseEventArgs e)
        {
            MouseDownOverBoardFlag = false;
            e.Handled = true;
        }

        private void Container_MouseMove(object sender, MouseEventArgs e)
        {
            if (MovingSelected)
            {
                foreach (IMovable m in ls)
                {
                    m.Move(BoardRect, this, e);
                }
                UpdateAllAnchorPixels();
                UpdateAngleCenter();
                OnAnchorPixelChanged();
            }

            else if (((IMovable)AngleVertex1).Moving)
            {
                AngleVertex1.Move(BoardRect, this, e);
                UpdateLinearAngle();
                OnLegacyParametersChanged();
            }

            else if (((IMovable)AngleVertex2).Moving)
            {
                AngleVertex2.Move(BoardRect, this, e);
                UpdateLinearAngle();
                OnLegacyParametersChanged();
            }

            else if (((IMovable)AngleCenterControl1).Moving)
            {
                AngleCenterControl1.Move(BoardRect, this, e);
                UpdateAngleCenter();
                OnLegacyParametersChanged();
            }

            else if (MouseDownOverAnchPixFlag)
            {
                CurrentAnchorPixel.Move(BoardRect, this, e);
                UpdateAnchorPixelLocation(CurrentAnchorPixel);
                OnAnchorPixelChanged();
            }
   
            else if (MouseDownOverBoardFlag && ((Keyboard.GetKeyStates(Key.Space) != KeyStates.None && Keyboard.GetKeyStates(Key.Space) != KeyStates.Toggled) || CMode == CursorMode.Dragging))
            {
                if (CMode != CursorMode.Dragging) Mouse.SetCursor(Cursors.Hand);

                double dragOffsetX = e.GetPosition(Container).X - BoardDragOffset.X;
                double dragOffsetY = e.GetPosition(Container).Y - BoardDragOffset.Y;
                double a;

                int mx, nx, my, ny;
                mx = (int)(0.5 * ImageSize.X - Constants.WorkingSurfaceSize / 2 - 200 / ZoomFactor);
                nx = (int)(0.5 * ImageSize.X + Constants.WorkingSurfaceSize / 2 - ProxyState.ViewportSizeH / ZoomFactor + 200 / ZoomFactor);
                my = (int)(0.5 * ImageSize.Y - Constants.WorkingSurfaceSize / 2 - 200 / ZoomFactor);
                ny = (int)(0.5 * ImageSize.Y + Constants.WorkingSurfaceSize / 2 - ProxyState.ViewportSizeV / ZoomFactor + 200 / ZoomFactor);

                if (ProxyState.ViewportSizeH / ZoomFactor > Constants.WorkingSurfaceSize)
                {
                    dragOffsetX = 0;
                }
                else
                {
                    a = vlX - (Int32)(dragOffsetX / ZoomFactor);
                    if (a < mx) dragOffsetX = (vlX - mx) * ZoomFactor;
                    else if (a > nx) dragOffsetX = (vlX - nx) * ZoomFactor;
                    else dragOffsetX = (vlX - a) * ZoomFactor;
                }

                if (ProxyState.ViewportSizeV / ZoomFactor > Constants.WorkingSurfaceSize)
                {
                    dragOffsetY = 0;
                }
                else
                {
                    a = vlY - (Int32)(dragOffsetY / ZoomFactor);
                    if (a < my) dragOffsetY = (vlY - my) * ZoomFactor;
                    else if (a > ny) dragOffsetY = (vlY - ny) * ZoomFactor;
                    else dragOffsetY = (vlY - a) * ZoomFactor;
                }

                for (Int32 i = 0; i <= AnchorPixels.Count - 1; i++)
                {
                    Canvas.SetTop(AnchorPixels[i], p[i].Y + dragOffsetY);
                    Canvas.SetLeft(AnchorPixels[i], p[i].X + dragOffsetX);
                }

                Canvas.SetTop(AngleCenterControl1, acY + dragOffsetY);
                Canvas.SetLeft(AngleCenterControl1, acX + dragOffsetX);
                ViewportLocationX = vlX - (Int32)(dragOffsetX / ZoomFactor);
                ViewportLocationY = vlY - (Int32)(dragOffsetY / ZoomFactor);
                BoardRect.Location = new Point(((-Constants.WorkingSurfaceSize + ImageSize.X) / 2 - ViewportLocationX) * ZoomFactor - Constants.AnchPixelControlDragOffset, ((-Constants.WorkingSurfaceSize + ImageSize.Y) / 2 - ViewportLocationY) * ZoomFactor - Constants.AnchPixelControlDragOffset);

                UpdateAllAnchorPixels();
                OnProxyStateChanged();
            }
            e.Handled = true;
        }

        private void Container_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            ProxyState.ViewportSizeH = (Int32)Container.ActualWidth;
            ProxyState.ViewportSizeV = (Int32)Container.ActualHeight;
            OnProxyStateChanged();
        }

        private void UserControl_Drop(object sender, DragEventArgs e)
        {
            Color color   = (Color)(e.Data.GetData("Color"));
            Byte  opacity = (Byte) (e.Data.GetData("Opacity"));
            AddAnchorPixel((Int32)e.GetPosition(Container).X, (Int32)e.GetPosition(Container).Y, CoordinateContext.Screen, color, opacity, true);
        }
    }

}
