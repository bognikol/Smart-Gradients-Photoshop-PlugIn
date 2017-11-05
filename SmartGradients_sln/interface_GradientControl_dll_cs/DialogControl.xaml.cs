using System;
using System.Windows;
using System.Windows.Controls;


namespace GradientControls
{
    internal partial class DialogControl : UserControl
    {
        #region INTERNAL INTERFACE

        public DialogControl()
        {
            InitializeComponent();
        }

        internal event EventHandler OKPressed;
        internal event EventHandler CancelPressed;

        #endregion

        protected void OnOKPressed(Object sender, EventArgs e)
        {
            if (OKPressed != null)
            {
                OKPressed(sender, e);
            }
        }

        protected void OnCancelPressed(Object sender, EventArgs e)
        {
            if (CancelPressed != null)
            {
                CancelPressed(sender, e);
            }
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            OnOKPressed(this, EventArgs.Empty);
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            OnCancelPressed(this, EventArgs.Empty);
        }
    }
}
