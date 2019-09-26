using System;
using System.Collections;
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
            this.paretocalculator = new ParetoCalculatorW();
        }

        private ParetoCalculatorW paretocalculator;

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
                    this.paretocalculator.LoadFile(openFileDialog.FileName);
                    this.paretocalculator.LoadQuantityTypes();
                    this.paretocalculator.LoadConfigurationSpaces();
                    this.paretocalculator.LoadConfigurationSets();
                }
               catch (ParetoCalculatorExceptionW exc) {
                    this.ParetoCalculatorExceptionOccurred(exc);
                }
                }
                this.updateStack();
            }

        private void ParetoCalculatorExceptionOccurred(ParetoCalculatorExceptionW e)
        {
            this.verbose("An exception occurred: " + e.Message + "\n");
            this.updateStack();
        }

        private void verbose(String s)
        {
            throw new NotImplementedException("Not implemented yet.");
        }   

        private void updateStack()
        {
            this.stackListView.Items.Clear();
            ArrayList l = this.paretocalculator.getStackItemStrings();
            foreach (String^ s in l)
            {
                this.stackListView.Items.Add(s);
            }
        }

    }



}
