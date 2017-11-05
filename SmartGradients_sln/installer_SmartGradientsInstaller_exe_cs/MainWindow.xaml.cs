using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using SmartGradientsInstaller.Controls;
using System.ComponentModel;

namespace SmartGradientsInstaller
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            licenceControl.AgreeStateChanged += licenceControl_AgreeStateChanged;
            SetContentControl(welcomeControl);
            PreviousButton.IsEnabled = false;
        }

        private Int32 Counter;

        private WelcomeControl           welcomeControl           = new WelcomeControl();
        private LicenceControl           licenceControl           = new LicenceControl();
        private InstanceSelectionControl instanceSelectionControl = new InstanceSelectionControl();
        private ProgressControl          progressControl          = new ProgressControl();
        private SuccessControl           successControl           = new SuccessControl();
        private FailureControl           failureControl           = new FailureControl();

        private void ExitInstaller()
        {
            Close();
            Application.Current.Shutdown();
        }

        private void Rectangle_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left) DragMove();
        }

        private void licenceControl_AgreeStateChanged(object sender, LicenceControl.LicenceEventArgs e)
        {
            if (Counter == 1) NextButton.IsEnabled = e.State;
        }

        private void PreviousButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Counter--;
                switch (Counter)
                {
                    case 0:
                        {
                            PreviousButton.IsEnabled = false;
                            NextButton.IsEnabled = true;
                            SetContentControl(welcomeControl);
                            break;
                        }
                    case 1:
                        {
                            SetContentControl(licenceControl);
                            break;
                        }
                    case 2:
                        {
                            SetContentControl(instanceSelectionControl);
                            break;
                        }
                    case 3:
                        {
                            SetContentControl(progressControl);
                            break;
                        }
                    case 4:
                        {
                            NextButton.IsEnabled = true;
                            SetContentControl(successControl);
                            break;
                        }
                    case 5:
                        {
                            NextButton.IsEnabled = false;
                            SetContentControl(failureControl);
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }
            }
            catch (Exception ex)
            {
                ShowFailure(ex.Message);
            }
        }

        private void NextButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Counter++;
                switch (Counter)
                {
                    case 0:
                        {
                            PreviousButton.IsEnabled = false;
                            SetContentControl(welcomeControl);
                            break;
                        }
                    case 1:
                        {
                            PreviousButton.IsEnabled = true;
                            NextButton.IsEnabled = (bool)licenceControl.AgreeChckBx.IsChecked;
                            SetContentControl(licenceControl);
                            break;
                        }
                    case 2:
                        {
                            PreviousButton.IsEnabled = true;
                            instanceSelectionControl.PhotoshopInstanceList = MainRoutines.GetCandidates(Environment.Is64BitOperatingSystem);
                            SetContentControl(instanceSelectionControl);
                            break;
                        }
                    case 3:
                        {
                            if (instanceSelectionControl.NumberOfSelectedInstances() == 0)
                            {
                                ShowFailure("Nothing selected for intalation. Installation is terminated.");
                            }
                            else
                            {
                                SetContentControl(progressControl);
                                NextButton.IsEnabled = false;
                                PreviousButton.IsEnabled = false;
                                progressControl.CurInstlTxtBx.Text = "Currently installing: Microsoft Visual C++ 2015 Redistributable";
                                progressControl.InstlProgrs.Value = 50;
                                if (!MainRoutines.InstallCppRedistributableV140(Environment.Is64BitOperatingSystem, new EventHandler(RedistributableHandler)))
                                {
                                    ShowFailure("Microsoft Visual C++ 2015 Redistributable installation failed. Installation is terminated.");
                                }
                            }
                            break;
                        }
                    case 4:
                        {
                            NextButton.Content = "Finish";
                            SetContentControl(successControl);
                            break;
                        }
                    case 5:
                        {
                            ExitInstaller();
                            break;
                        }
                    default:
                        break;

                }
            }
            catch (Exception ex)
            {
                ShowFailure(ex.Message);
            }
        }

        private void RedistributableHandler(Object sender, EventArgs e)
        {
            Int32 processReturnCode = ((System.Diagnostics.Process)sender).ExitCode;

            if (processReturnCode == 0 || processReturnCode == 1638)
            {
                Application.Current.Dispatcher.Invoke(
                    () =>
                    {
                        progressControl.CurInstlTxtBx.Text = "Currently installing: Smart Gradients Beta plug-in for Adobe® Photoshop®";
                        progressControl.InstlProgrs.Value = 80;
                        System.Threading.Thread.Sleep(5000);
                        foreach (PhotoshopInstallationInstance pi in instanceSelectionControl.PhotoshopInstanceList)
                        {
                            try
                            {
                                MainRoutines.InstallPlugIn(pi.Path, pi.Is64BitPhotoshop());
                                pi.InstallationSuccesfull = true;
                            }
                            catch (Exception ex)
                            {
                                pi.InstallationSuccesfull = false;
                                pi.ExceptionIfFailure = ex;
                            }
                        }
                        NextButton.IsEnabled = true;
                        CancelButton.IsEnabled = false;
                        NextButton_Click(NextButton, new RoutedEventArgs());
                    });
            }
            else
            {
                Application.Current.Dispatcher.Invoke(
                () =>
                {
                    ShowFailure("Installation of Microsoft Visual C++ 2015 Redistributable failed with return code: " + processReturnCode.ToString());
                });
            }

        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            ExitInstaller();
        }

        private void Label_MouseUp(object sender, MouseButtonEventArgs e)
        {
            ExitInstaller();
        }

        public void SetContentControl(Control control)
        {
            InnterContent.Children.Clear();
            InnterContent.Children.Add(control);
        }

        private void ShowFailure(String message)
        {
            failureControl.FailureMsg.Text = "Installation of Smart Gradients Beta plugin for Adobe® Photoshop® failed with following message: " + message;
            SetContentControl(failureControl);
            PreviousButton.IsEnabled = false;
            NextButton.IsEnabled = false;
            CancelButton.IsEnabled = true;
            CancelButton.Content = "Finish";

        }


    }
}
