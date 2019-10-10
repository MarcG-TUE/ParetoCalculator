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
using System.Windows.Shapes;

namespace ParetoCalculatorUI.Dialogs
{
    /// <summary>
    /// Interaction logic for StoreDialog.xaml
    /// </summary>
    public partial class StoreDialog : Window
    {
        public StoreDialog(String suggestedName)
        {
            InitializeComponent();
            this.nameEditBox.Text = suggestedName;
        }

        public string NameResult;

        private void StoreButton_Click(object sender, RoutedEventArgs e)
        {
            this.NameResult = this.nameEditBox.Text;
            this.DialogResult = true;
            this.Close();
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
            this.Close();
        }
    }
}
