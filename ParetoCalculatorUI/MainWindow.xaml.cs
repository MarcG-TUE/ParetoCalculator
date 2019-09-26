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
using ParetoCalculatorUI.Dialogs;

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

            AboutDialog dialogBox = new AboutDialog();
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
                catch (ParetoCalculatorExceptionW exc)
                {
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
            this.consoleText.Text += s;
            this.consoleText.Text += System.Environment.NewLine;
            this.consoleText.ScrollToEnd();
        }

        private void updateStack()
        {
            this.stackListView.Items.Clear();
            ArrayList l = this.paretocalculator.getStackItemStrings();
            foreach (String s in l)
            {
                this.stackListView.Items.Add(s);
            }
        }

        private void executeButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                this.paretocalculator.LoadOperations();
                this.updateStack();
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.updateStack();
            }
        }

        private void pushButton_Click(object sender, RoutedEventArgs e)
        {
            SelectionDialog dialogBox = new SelectionDialog(paretocalculator);
            Nullable<bool> dialogResult = dialogBox.ShowDialog();

            if (dialogResult == true)
            {
                string s = (string) dialogBox.selectionListbox.SelectedItem;
                this.paretocalculator.push(s);
            }
            this.updateStack();
        }

        private void popButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (this.paretocalculator.stackSize() > 0)
                {
                    string s = this.paretocalculator.pop();
                    this.verbose(s);
                    this.updateStack();
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.updateStack();
            }
        }

        private void storeButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!this.paretocalculator.stackEmpty())
                {
                    StoreDialog dialogBox = new StoreDialog(this.paretocalculator.peek());
                    Nullable<bool> dialogResult = dialogBox.ShowDialog();

                    if (dialogResult == true)
                    {
                        this.paretocalculator.storePop(dialogBox.NameResult);
                        this.updateStack();
                    }
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.updateStack();
            }
        }

        private void printButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // print selected item from stack
                int n = this.stackListView.SelectedIndex;
                if ((n >= 0) && (n < this.paretocalculator.stackSize()))
                {
//                    StorableObject* o = stack[n];
                    this.verbose(this.paretocalculator.peek(n));
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.updateStack();
            }
        }

        private void duplicateButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!this.paretocalculator.stackEmpty())
                {
                    this.paretocalculator.duplicate();
                    this.updateStack();
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.updateStack();
            }
        }

        private void productButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (this.paretocalculator.stackSize() >= 2)
                {
                    this.paretocalculator.product();
                    this.updateStack();
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.updateStack();
            }
        }

        private void prodconsButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!this.paretocalculator.stackEmpty())
                {
                    ProdConsDialog dialogBox = new ProdConsDialog(this.paretocalculator.peek());
                    Nullable<bool> dialogResult = dialogBox.ShowDialog();

                    if (dialogResult == true)
                    {
                        string pq = dialogBox.prodCombo.SelectedItem;
                        string cq = dialogBox.consCombo.SelectedItem;
                        this.paretocalculator.executeProdCons(pq, cq);
                        this->updateStack();
                        delete & pco;
                    }
                }
            }
            catch (EParetoCalculatorError&e) {
                this->ParetoCalculatorExceptionOccurred(e);
            }
            }
        }

}
