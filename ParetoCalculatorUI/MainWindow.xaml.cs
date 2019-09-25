using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Win32;

namespace ParetoCalculatorUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void aboutButton_Click(object sender, RoutedEventArgs e)
        {
            ParetoCalculatorW pc = new ParetoCalculatorW();

            // Instantiate window
            AboutDialog dialogBox = new AboutDialog();

            // Show window modally
            // NOTE: Returns only when window is closed
            Nullable<bool> dialogResult = dialogBox.ShowDialog();
        }

        private void fileSelectButton_Click(object sender, RoutedEventArgs e)
        {

            OpenFileDialog openFileDialog = new OpenFileDialog();

            openFileDialog.Filter = "xml files (*.xml)|*.xml|All files (*.*)|*.*";
            openFileDialog.FilterIndex = 2;
            openFileDialog.RestoreDirectory = true;

            bool? result = openFileDialog.ShowDialog();
            if (result == true)
            {
                try
                {
                    pc->LoadFile(openFileDialog.FileName);
                    pc->LoadQuantityTypes();
                    pc->LoadConfigurationSpaces();
                    pc->LoadConfigurationSets();
                }
                catch (EParetoCalculatorErrorW&e) {
                    this->ParetoCalculatorExceptionOccurred(e);
                }
                }
                this->updateStack();
            }
        }
}
