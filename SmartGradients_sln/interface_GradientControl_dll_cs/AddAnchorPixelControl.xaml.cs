using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;


namespace GradientControls
{
    internal partial class AddAnchorPixelControl : UserControl
    {
        #region INTERNAL INTERFACE

        public AddAnchorPixelControl()
        {
            InitializeComponent();
            AnchorPixelToBeAdded.ContextMenu = null;
        }

        internal event EventHandler ColorChangeRequested;


        internal Boolean OpacityAllowed
        {
            get { return SetOpacityButton.IsEnabled;  }
            set { SetOpacityButton.IsEnabled = value; } 
        }

        #endregion

        protected void OnColorChangeRequested()
        {
            if (ColorChangeRequested != null)
            {
                ColorChangeRequested(this, EventArgs.Empty);
            }
        }

        private void AnchorPixelControl_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            OnColorChangeRequested();
        }

        private void AnchorPixelToBeAdded_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                DataObject data = new DataObject();
                data.SetData("Color",   AnchorPixelToBeAdded.Color);
                data.SetData("Opacity", AnchorPixelToBeAdded.AnchPixOpacity);
                DragDrop.DoDragDrop(AnchorPixelToBeAdded, data, DragDropEffects.All);
            }
        }

        private void SetColorButton_Click(object sender, RoutedEventArgs e)
        {
            OnColorChangeRequested();
        }

        private void SetOpacityButton_Click(object sender, RoutedEventArgs e)
        {
            SetOpacityWindow setOpacityWindow = new SetOpacityWindow();
            setOpacityWindow.ShowDialog(AnchorPixelToBeAdded.AnchPixOpacity);
            if (setOpacityWindow.DialogResult == true) AnchorPixelToBeAdded.AnchPixOpacity = setOpacityWindow.AnchPixelOpacity;

        }
    }
}
