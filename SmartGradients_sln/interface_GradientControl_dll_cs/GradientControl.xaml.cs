using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Threading;

namespace GradientControls
{

    public class m_AnchorPixel
    {
        public Int32 X;
        public Int32 Y;
        public Byte CH0;
        public Byte CH1;
        public Byte CH2;
        public Byte CH3;
        public Byte CH4;

        public Color WPFColor()
        {
            return Color.FromArgb(255, CH0, CH1, CH2);
        }

    }

    public class m_GradientState
    {

        public const Int32 ParameterNotUsed = -100;

        public m_GradientState()
        {
            for (Int32 i = 0; i <= 24; i++)
            {
                AnchPixs[i] = new m_AnchorPixel();
            }
        }

        //GENERAL MEMBERS
        public Boolean OpacityApplied = false;
        public Int32 Type = 0;
        public m_AnchorPixel[] AnchPixs = new m_AnchorPixel[25];
        public Int32 ActiveAnchPixs = 3;

        //COMPLEX SPECIFIC MEMBERS
        public Byte Px = 1;
        public Byte Py = 1;
        public Double DiffusionIndex = 1.0;
        public Int32 Parameter = ParameterNotUsed;


        public Double Grad;

        //LINEAR SPECIFIC MEMBERS
        public Double Angle = 1;

        //RADIAL SPECIFIC MEMBERS
        public Int32 CentralPix = 0;

        //ANGLED SPECIFIC MEMEBERS
        public Int32 CenterX = 200;
        public Int32 CenterY = 200;
    }

    public class m_ProxyState
    {
        public Int32 ViewportLocationH = 0;
        public Int32 ViewportLocationV = 0;
        public Int32 ViewportSizeH = 20;
        public Int32 ViewportSizeV = 20;
        public Double ZoomFactor = 1.0;
    }

    internal class Constants
    {
        static internal Int32 WorkingSurfaceSize = 10000;
        static internal Int32 AnchPixelControlDragOffset = 15;
        static internal Int32 NonRealTimeMode_EventDefferingInterval_InMiliseconds = 100;
        static internal Int32 PreviewBitmapWidth  = (Int32)SystemParameters.MaximizedPrimaryScreenWidth;
        static internal Int32 PreviewBitmapHeight = (Int32)SystemParameters.MaximizedPrimaryScreenHeight;
    }

    public partial class GradinetControl : UserControl
    {

        #region PUBLIC INTERFACE

        public GradinetControl(Int32 imageWidth, Int32 imageHeight, Boolean opacityAllowed)
        {
            InitializeComponent();

            Result = 0;

            mergedChannelsBitmap = new WriteableBitmap(Constants.PreviewBitmapWidth, Constants.PreviewBitmapHeight, 96, 96, PixelFormats.Bgra32, null);
            gradinetBitmap       = new WriteableBitmap(Constants.PreviewBitmapWidth, Constants.PreviewBitmapHeight, 96, 96, PixelFormats.Bgra32, null);

            GradientState = new m_GradientState();
            GradientState.ActiveAnchPixs = 3;

            ProxyState = new m_ProxyState();
            ProxyState.ViewportLocationH = 0;
            ProxyState.ViewportLocationV = 0;
            ProxyState.ZoomFactor = 1.0;

            pixelBoardControl = new PixelBoardControl(GradientState, ProxyState, new Point(imageWidth, imageHeight), opacityAllowed);
            pixelBoardControl.GradientState = GradientState;
            pixelBoardControl.AnchorPixelRequestedColorChange += pixelBoardControl_AnchorPixelRequestedColorChange;
            pixelBoardControl.AnchorPixelChanged              += pixelBoardControl_AnchorPixelChanged;
            pixelBoardControl.ProxyStateChanged               += pixelBoardControl_ProxyStateChanged;
            pixelBoardControl.NumberOfAnchorPixelsChanged     += pixelBoardControl_NumberOfAnchorPixelsChanged;
            pixelBoardControl.LegacyParametersChanged         += pixelBoardControl_LegacyParametersChanged;
            mainGrid.Children.Add(pixelBoardControl);

            masterControl.GradientState = GradientState;
            masterControl.GradientStateChanged += masterControl_GradientStateChanged;

            addAnchorPixelControl.OpacityAllowed = opacityAllowed;

            ChannelsImage.Source = mergedChannelsBitmap;
            GradientImage.Source = gradinetBitmap;


            gradientUpdateTimer = new DispatcherTimer();
            gradientUpdateTimer.Interval = new TimeSpan(0, 0, 0, 0, Constants.NonRealTimeMode_EventDefferingInterval_InMiliseconds);
            gradientUpdateTimer.Tick += gradientUpdateTimer_Tick;


            Double zoomX, zoomY;
            zoomX = (SystemParameters.MaximizedPrimaryScreenWidth  - 400) / imageWidth;
            zoomY = (SystemParameters.MaximizedPrimaryScreenHeight - 100) / imageHeight;

            if   (zoomX < zoomY) zoomControl.FitZoom = zoomX;
            else                 zoomControl.FitZoom = zoomY;

            if (zoomControl.FitZoom < 1) zoomControl.ZoomComboBox.SelectedIndex = 12;


            ImageBrush backgroundImageBrush = new ImageBrush(getBackgroundBitmap());
            backgroundImageBrush.TileMode = TileMode.Tile;
            backgroundImageBrush.Viewport = new Rect(0, 0, 16, 16);
            backgroundImageBrush.ViewportUnits = BrushMappingMode.Absolute;
            backgroundImageBrush.Stretch = Stretch.None;
            ImageRect.Fill = backgroundImageBrush;
        }


        public class GradientControlEventArgs : EventArgs
        {
            public AnchorPixelControl ReferenceAnchPixel;
            public Boolean ProxyStateChanged;
        }


        public delegate void GradientControlDelegate(GradinetControl sender, GradientControlEventArgs e);

        public event GradientControlDelegate ContinuationRequested;
        public event GradientControlDelegate CancelationRequested;
        public event GradientControlDelegate ProxyUpdateRequested;
        public event GradientControlDelegate ColorDialogRequested;


        public Int32 Result;
        public m_GradientState GradientState;
        public m_ProxyState ProxyState;


        unsafe public void LockProxyBuffers(ref Byte* addressMerged, ref Byte* addressGradient, ref Int32 stride)
        {
            mergedChannelsBitmap.Lock();
            gradinetBitmap      .Lock();

            addressMerged   = (Byte*)mergedChannelsBitmap.BackBuffer.ToPointer();
            addressGradient = (Byte*)gradinetBitmap      .BackBuffer.ToPointer();

            stride = mergedChannelsBitmap.BackBufferStride;
        }

        unsafe public void UnlockProxyBuffers(Int32 width, Int32 height)
        {
            mergedChannelsBitmap.AddDirtyRect(new Int32Rect(0, 0, width, height));
            mergedChannelsBitmap.Unlock();

            gradinetBitmap.AddDirtyRect(new Int32Rect(0, 0, width, height));
            gradinetBitmap.Unlock();

            ChannelsImage.Width  = width;
            ChannelsImage.Height = height;
            GradientImage.Width  = width;
            GradientImage.Height = height;
            ImageRect.Width  = width;
            ImageRect.Height = height;

            double verticalOffset, horizontalOffset;
            horizontalOffset = -ProxyState.ViewportLocationH * ProxyState.ZoomFactor;
            verticalOffset   = -ProxyState.ViewportLocationV * ProxyState.ZoomFactor;
            if (horizontalOffset < 0) horizontalOffset = 0;
            if (verticalOffset   < 0) verticalOffset   = 0;
            if (horizontalOffset > ProxyState.ViewportSizeH) horizontalOffset = ProxyState.ViewportSizeH;
            if (verticalOffset   > ProxyState.ViewportSizeV) verticalOffset   = ProxyState.ViewportSizeV;

            Canvas.SetTop (PixelMoveRect, pixelBoardControl.BoardRect.Top  - 2);
            Canvas.SetLeft(PixelMoveRect, pixelBoardControl.BoardRect.Left - 2);
            PixelMoveRect.Width  = pixelBoardControl.BoardRect.Width  + 32;
            PixelMoveRect.Height = pixelBoardControl.BoardRect.Height + 32;

            Canvas.SetTop (ChannelsImage, verticalOffset);
            Canvas.SetLeft(ChannelsImage, horizontalOffset);
            Canvas.SetTop (GradientImage, verticalOffset);
            Canvas.SetLeft(GradientImage, horizontalOffset);
            Canvas.SetTop (ImageRect,     verticalOffset);
            Canvas.SetLeft(ImageRect,     horizontalOffset);
        }

        #endregion



        private WriteableBitmap mergedChannelsBitmap;
        private WriteableBitmap gradinetBitmap;
        private PixelBoardControl pixelBoardControl;
        private DispatcherTimer gradientUpdateTimer;
        private GradientControlEventArgs storedGradientControlEventArgs;


        protected virtual void OnContinuationRequested(GradientControlEventArgs e)
        {
            if (ContinuationRequested != null)
            {
                ContinuationRequested(this, e);
            }
        }

        protected virtual void OnCancelationRequested(GradientControlEventArgs e)
        {
            if (CancelationRequested != null)
            {
                CancelationRequested(this, e);
            }
        }

        protected virtual void OnProxyUpdateRequested(GradientControlEventArgs e)
        {
            if (ProxyUpdateRequested != null)
            {
                ProxyUpdateRequested(this, e);
            }
        }

        protected virtual void OnColorDialogRequested(GradientControlEventArgs e)
        {
            if (ColorDialogRequested != null)
            {
                ColorDialogRequested(this, e);
            }
        }




        private void gradientUpdateTimer_Tick(object sender, EventArgs e)
        {
            OnProxyUpdateRequested(storedGradientControlEventArgs);
        }

        private void pixelBoardControl_AnchorPixelChanged(Object sender, EventArgs e)
        {
            GradientControlEventArgs gradientControlEventArgs = new GradientControlEventArgs();
            gradientControlEventArgs.ProxyStateChanged = false;

            if (RealtimePreviewChckBx.IsChecked == true)
            {
                OnProxyUpdateRequested(gradientControlEventArgs);
            }
            else
            {
                storedGradientControlEventArgs = gradientControlEventArgs;
                gradientUpdateTimer.Stop();
                gradientUpdateTimer.Start();
            }
        }

        private void pixelBoardControl_ProxyStateChanged(Object sender, EventArgs e)
        {
            UpdateScrollBars();

            GradientControlEventArgs gradientControlEventArgs = new GradientControlEventArgs();
            gradientControlEventArgs.ProxyStateChanged = true;

            if (RealtimePreviewChckBx.IsChecked == true)
            {
                OnProxyUpdateRequested(gradientControlEventArgs);
            }
            else
            {
                storedGradientControlEventArgs = gradientControlEventArgs;
                gradientUpdateTimer.Stop();
                gradientUpdateTimer.Start();
            }
        }

        private void pixelBoardControl_NumberOfAnchorPixelsChanged(Object sender, EventArgs e)
        {
            masterControl.AnchPixNum = GradientState.ActiveAnchPixs;
        }

        private void pixelBoardControl_LegacyParametersChanged(Object sender, EventArgs e)
        {
            GradientControlEventArgs gradientControlEventArgs = new GradientControlEventArgs();
            gradientControlEventArgs.ProxyStateChanged = false;

            if (RealtimePreviewChckBx.IsChecked == true)
            {
                OnProxyUpdateRequested(gradientControlEventArgs);
            }
            else
            {
                storedGradientControlEventArgs = gradientControlEventArgs;
                gradientUpdateTimer.Stop();
                gradientUpdateTimer.Start();
            }
        }

        private void masterControl_GradientStateChanged(object sender, EventArgs e)
        {
            pixelBoardControl.ShowAppropriateControls();

            GradientControlEventArgs gradientControlEventArgs = new GradientControlEventArgs();
            gradientControlEventArgs.ProxyStateChanged = false;

            if (RealtimePreviewChckBx.IsChecked == true)
            {
                OnProxyUpdateRequested(gradientControlEventArgs);
            }
            else
            {
                storedGradientControlEventArgs = gradientControlEventArgs;
                gradientUpdateTimer.Stop();
                gradientUpdateTimer.Start();
            }
        }

        private void zoomControl_ZoomChanged(object sender, EventArgs e)
        {
            pixelBoardControl.ZoomFactor = zoomControl.Zoom;
        }

        private void dialogControl_OKPressed(object sender, EventArgs e)
        {
            GradientControlEventArgs gradientControlEventArgs = new GradientControlEventArgs();
            gradientUpdateTimer.Stop();
            OnContinuationRequested(gradientControlEventArgs);
        }

        private void dialogControl_CancelPressed(object sender, EventArgs e)
        {
            GradientControlEventArgs gradientControlEventArgs = new GradientControlEventArgs();
            gradientUpdateTimer.Stop();
            OnCancelationRequested(gradientControlEventArgs);
        }

        private void pixelBoardControl_AnchorPixelRequestedColorChange(object sender, EventArgs e)
        {
            GradientControlEventArgs gradientControlEventArgs = new GradientControlEventArgs();
            gradientControlEventArgs.ReferenceAnchPixel = pixelBoardControl.CurrentAnchorPixel;
            OnColorDialogRequested(gradientControlEventArgs);
        }

        private void UserControl_KeyDown(object sender, KeyEventArgs e)
        {
            if (Keyboard.GetKeyStates(Key.LeftCtrl) != KeyStates.None || Keyboard.GetKeyStates(Key.LeftCtrl) != KeyStates.Toggled)
            {
                if      (e.Key == Key.Add      || e.Key == Key.OemPlus)  zoomControl.ZoomIn();
                else if (e.Key == Key.Subtract || e.Key == Key.OemMinus) zoomControl.ZoomOut();
            }
        }

        private void UserControl_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Space || e.Key == Key.Return)
            {
                e.Handled = true;
            }
        }

        private void addAnchorPixelControl_ColorChangeRequested(object sender, EventArgs e)
        {
            GradientControlEventArgs gradientControlEventArgs = new GradientControlEventArgs();
            gradientControlEventArgs.ReferenceAnchPixel = ((AddAnchorPixelControl)sender).AnchorPixelToBeAdded;
            OnColorDialogRequested(gradientControlEventArgs);
        }

        private void cursorRadioButtonArrow_Checked(object sender, RoutedEventArgs e)
        {
            if (IsInitialized)
            {
                if (CursorRadioButtonHand.IsChecked == true)
                {
                    pixelBoardControl.CMode = PixelBoardControl.CursorMode.Dragging;
                }
                else
                {
                    pixelBoardControl.CMode = PixelBoardControl.CursorMode.Arrow;
                }
            }
        }

        private Boolean SuspendScrollUpdate;

        private void UpdateScrollBars()
        {
            if (!SuspendScrollUpdate)
            {
                SuspendScrollUpdate = true;

                Double sizeInImagePixels;

                sizeInImagePixels = ProxyState.ViewportSizeH / ProxyState.ZoomFactor;
                if (sizeInImagePixels < Constants.WorkingSurfaceSize)
                {
                    HorizontalScroll.ViewportSize = sizeInImagePixels;
                    HorizontalScroll.Maximum = Constants.WorkingSurfaceSize + 400 / ProxyState.ZoomFactor - sizeInImagePixels;
                }
                else
                {
                    HorizontalScroll.ViewportSize = Constants.WorkingSurfaceSize;
                    HorizontalScroll.Maximum = 0;
                }

                HorizontalScroll.Value = ProxyState.ViewportLocationH + Constants.WorkingSurfaceSize / 2 + 200 / ProxyState.ZoomFactor - pixelBoardControl.ImageSize.X / 2;

                sizeInImagePixels = ProxyState.ViewportSizeV / ProxyState.ZoomFactor;
                if (sizeInImagePixels < Constants.WorkingSurfaceSize)
                {
                    VerticalScroll.ViewportSize = sizeInImagePixels;
                    VerticalScroll.Maximum = Constants.WorkingSurfaceSize + 400 / ProxyState.ZoomFactor - sizeInImagePixels;
                }
                else
                {
                    VerticalScroll.ViewportSize = Constants.WorkingSurfaceSize;
                    VerticalScroll.Maximum = 0;
                }

                VerticalScroll.Value = ProxyState.ViewportLocationV + Constants.WorkingSurfaceSize / 2 + 200 / ProxyState.ZoomFactor - pixelBoardControl.ImageSize.Y / 2;

                SuspendScrollUpdate = false;
            }
        }

        private void HorizontalScroll_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (IsInitialized && !SuspendScrollUpdate)
            {
                SuspendScrollUpdate = true;
                ProxyState.ViewportLocationH = (Int32)(HorizontalScroll.Value - Constants.WorkingSurfaceSize / 2 - 200 / ProxyState.ZoomFactor + pixelBoardControl.ImageSize.X / 2);
                pixelBoardControl.Redrag();
                SuspendScrollUpdate = false;
            }
        }

        private void VerticalScroll_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (IsInitialized && !SuspendScrollUpdate)
            {
                SuspendScrollUpdate = true;
                ProxyState.ViewportLocationV = (Int32)(VerticalScroll.Value - Constants.WorkingSurfaceSize / 2 - 200 / ProxyState.ZoomFactor + pixelBoardControl.ImageSize.Y / 2);
                pixelBoardControl.Redrag();
                SuspendScrollUpdate = false;
            }
        }

        private unsafe static void drawPixel(Byte color, ref Byte* destination)
        {
            *destination = color; destination++;
            *destination = color; destination++;
            *destination = color; destination++;
            *destination = 255;   destination++;
        }

        private unsafe static WriteableBitmap getBackgroundBitmap()
        {
            WriteableBitmap backgorundBitmap = new WriteableBitmap(16, 16, 96, 96, PixelFormats.Bgra32, null);
            backgorundBitmap.Lock();

            Byte* buff = (Byte*)(backgorundBitmap.BackBuffer);
            int stride = backgorundBitmap.BackBufferStride;
            for (int i = 0; i <= 7; i++)
            {
                for (int j = 0; j <= 7;  j++) drawPixel(255, ref buff);
                for (int j = 8; j <= 15; j++) drawPixel(204, ref buff);
            }
            for (int i = 8; i <= 15; i++)
            {
                for (int j = 0; j <= 7;  j++) drawPixel(204, ref buff);
                for (int j = 8; j <= 15; j++) drawPixel(255, ref buff);
            }

            backgorundBitmap.AddDirtyRect(new Int32Rect(0, 0, 16, 16));
            backgorundBitmap.Unlock();
            return backgorundBitmap;
        }
    }
}