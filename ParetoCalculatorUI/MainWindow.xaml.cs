using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
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
            this.paretocalculator.setStatusCallbacks(s => { this.setStatus_extthread(s); }, s => { this.verbose_extthread(s); }, 100);

        }

        private void setStatus(string s)
        {
            this.statusContent.Text = s;
        }

        private void setStatus_extthread(string s)
        {
            this.Dispatcher.Invoke(new MethodInvoker(()=>this.setStatus(s)));
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

            System.Windows.Forms.DialogResult result = openFileDialog.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
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
            //this.consoleText.Text += System.Environment.NewLine;
            this.consoleText.ScrollToEnd();
        }

        private void verbose_extthread(string s)
        {
            this.Dispatcher.Invoke(new MethodInvoker(() => this.verbose(s)));
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
                    this.verbose(this.paretocalculator.peek(paretocalculator.stackSize()-n-1));
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
                    ProdConsDialog dialogBox = new ProdConsDialog(this.paretocalculator);
                    Nullable<bool> dialogResult = dialogBox.ShowDialog();

                    if (dialogResult == true)
                    {
                        string pq = (string) dialogBox.prodCombo.SelectedItem;
                        string cq = (string) dialogBox.consCombo.SelectedItem;
                        this.paretocalculator.executeProdCons(pq, cq);
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

        private void abstractButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!this.paretocalculator.stackEmpty())
                {
                    AbstractDialog dialogBox = new AbstractDialog(this.paretocalculator);
                    Nullable<bool> dialogResult = dialogBox.ShowDialog();

                    if (dialogResult == true)
                    {
                        string aq = (string)dialogBox.abstractCombo.SelectedItem;
                        this.paretocalculator.executeAbstract(aq);
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

        private void joinButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!this.paretocalculator.stackEmpty())
                {
                    JoinDialog dialogBox = new JoinDialog(this.paretocalculator);
                    Nullable<bool> dialogResult = dialogBox.ShowDialog();

                    if (dialogResult == true)
                    {
                        string q = (string)dialogBox.quantCombo.SelectedItem;
                        this.paretocalculator.executeJoin(q);
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

        private void minimizeButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!this.paretocalculator.stackEmpty())
                {
                    this.paretocalculator.minimize();
                    this.updateStack();
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.updateStack();
            }
        }

        private void sumButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!this.paretocalculator.stackEmpty())
                {
                    DerivedOperatorDialog dialogBox = new DerivedOperatorDialog(this.paretocalculator, "Sum of quantities");
                    Nullable<bool> dialogResult = dialogBox.ShowDialog();

                    if (dialogResult == true)
                    {
                        string qa = (string)dialogBox.quantACombo.SelectedItem;
                        string qb = (string)dialogBox.quantBCombo.SelectedItem;
                        this.paretocalculator.executeSum(qa, qb);
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

        private void maxButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!this.paretocalculator.stackEmpty())
                {
                    DerivedOperatorDialog dialogBox = new DerivedOperatorDialog(this.paretocalculator, "Maximum of quantities");
                    Nullable<bool> dialogResult = dialogBox.ShowDialog();

                    if (dialogResult == true)
                    {
                        string qa = (string)dialogBox.quantACombo.SelectedItem;
                        string qb = (string)dialogBox.quantBCombo.SelectedItem;
                        this.paretocalculator.executeMax(qa, qb);
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

        private void minButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!this.paretocalculator.stackEmpty())
                {
                    DerivedOperatorDialog dialogBox = new DerivedOperatorDialog(this.paretocalculator, "Minimum of quantities");
                    Nullable<bool> dialogResult = dialogBox.ShowDialog();

                    if (dialogResult == true)
                    {
                        string qa = (string)dialogBox.quantACombo.SelectedItem;
                        string qb = (string)dialogBox.quantBCombo.SelectedItem;
                        this.paretocalculator.executeMin(qa, qb);
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

        private void multiplyButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!this.paretocalculator.stackEmpty())
                {
                    DerivedOperatorDialog dialogBox = new DerivedOperatorDialog(this.paretocalculator, "Product of quantities");
                    Nullable<bool> dialogResult = dialogBox.ShowDialog();

                    if (dialogResult == true)
                    {
                        string qa = (string)dialogBox.quantACombo.SelectedItem;
                        string qb = (string)dialogBox.quantBCombo.SelectedItem;
                        this.paretocalculator.executeMultiply(qa, qb);
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
    }

}
