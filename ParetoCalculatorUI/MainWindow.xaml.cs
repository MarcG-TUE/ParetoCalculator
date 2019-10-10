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
            this.paretocalculator.setStatusCallbacks(s => { this.SetStatus_extthread(s); }, s => { this.Verbose_extthread(s); }, 100);

        }

        private void SetStatus(string s)
        {
            this.statusContent.Text = s;
        }

        private void SetStatus_extthread(string s)
        {
            this.Dispatcher.Invoke(new MethodInvoker(()=>this.SetStatus(s)));
        }

        private readonly ParetoCalculatorW paretocalculator;

        private void AboutButton_Click(object sender, RoutedEventArgs e)
        {
            AboutDialog dialogBox = new AboutDialog();
            dialogBox.ShowDialog();
        }

        private void FileSelectButton_Click(object sender, RoutedEventArgs e)
        {

            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
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
            }
            this.UpdateStack();
        }

        private void ParetoCalculatorExceptionOccurred(ParetoCalculatorExceptionW e)
        {
            this.Verbose("An exception occurred: " + e.Message + "\n");
            this.UpdateStack();
        }

        private void Verbose(String s)
        {
            this.consoleText.Text += s;
            //this.consoleText.Text += System.Environment.NewLine;
            this.consoleText.ScrollToEnd();
        }

        private void Verbose_extthread(string s)
        {
            this.Dispatcher.Invoke(new MethodInvoker(() => this.Verbose(s)));
        }

        private void UpdateStack()
        {
            this.stackListView.Items.Clear();
            ArrayList l = this.paretocalculator.getStackItemStrings();
            foreach (String s in l)
            {
                this.stackListView.Items.Add(s);
            }
        }

        private void ExecuteButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                this.paretocalculator.LoadOperations();
                this.UpdateStack();
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

        private void PushButton_Click(object sender, RoutedEventArgs e)
        {
            SelectionDialog dialogBox = new SelectionDialog(paretocalculator);
            Nullable<bool> dialogResult = dialogBox.ShowDialog();

            if (dialogResult == true)
            {
                string s = (string) dialogBox.selectionListbox.SelectedItem;
                this.paretocalculator.push(s);
            }
            this.UpdateStack();
        }

        private void PopButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (this.paretocalculator.stackSize() > 0)
                {
                    string s = this.paretocalculator.pop();
                    this.Verbose(s);
                    this.UpdateStack();
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

        private void StoreButton_Click(object sender, RoutedEventArgs e)
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
                        this.UpdateStack();
                    }
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

        private void PrintButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // print selected item from stack
                int n = this.stackListView.SelectedIndex;
                if ((n >= 0) && (n < this.paretocalculator.stackSize()))
                {
                    this.Verbose(this.paretocalculator.peek(paretocalculator.stackSize()-n-1));
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

        private void DuplicateButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!this.paretocalculator.stackEmpty())
                {
                    this.paretocalculator.duplicate();
                    this.UpdateStack();
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

        private void ProductButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (this.paretocalculator.stackSize() >= 2)
                {
                    this.paretocalculator.product();
                    this.UpdateStack();
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

        private void ProdconsButton_Click(object sender, RoutedEventArgs e)
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
                        this.UpdateStack();
                    }
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

        private void AbstractButton_Click(object sender, RoutedEventArgs e)
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
                        this.UpdateStack();
                    }
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

        private void JoinButton_Click(object sender, RoutedEventArgs e)
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
                        this.UpdateStack();
                    }
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

        private void MinimizeButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!this.paretocalculator.stackEmpty())
                {
                    this.paretocalculator.minimize();
                    this.UpdateStack();
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

        private void SumButton_Click(object sender, RoutedEventArgs e)
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
                        this.UpdateStack();
                    }
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

        private void MaxButton_Click(object sender, RoutedEventArgs e)
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
                        this.UpdateStack();
                    }
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

        private void MinButton_Click(object sender, RoutedEventArgs e)
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
                        this.UpdateStack();
                    }
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

        private void MultiplyButton_Click(object sender, RoutedEventArgs e)
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
                        this.UpdateStack();
                    }
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

        private void PlotButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!this.paretocalculator.stackEmpty())
                {
                    GraphDialog dialogBox = new GraphDialog(this.paretocalculator);
                    Nullable<bool> dialogResult = dialogBox.ShowDialog();
                }
            }
            catch (ParetoCalculatorExceptionW exc)
            {
                this.ParetoCalculatorExceptionOccurred(exc);
                this.UpdateStack();
            }
        }

    }

}
