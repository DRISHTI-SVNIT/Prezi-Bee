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
using System.IO.Ports;
using System.Windows.Forms;
namespace myproject
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


        SerialPort MyPort = new SerialPort("COM5", 9600);//Adjust the comp port
        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            String tx = TXWINDOW.Text;


            MyPort.Write(tx);
            TXWINDOW.Clear();
 
        }
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MyPort.IsOpen) MyPort.Close();
        }
        String RxString;
        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        { //  void delegate DisplayText();
            RxString = MyPort.ReadExisting();
           this.Invoke(new EventHandler(DisplayText));
          // DisplayText();
          
        }

        private void Invoke(EventHandler eventHandler)
        {
            throw new NotImplementedException();
        }
        private  void DisplayText(object sender, EventArgs e)
        {
            RXWINDOW.AppendText(RxString);


        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
           // SerialPort MyPort = new SerialPort("COM1", 9600);//Adjust the comp port
            MyPort.Open();
            if (!MyPort.IsOpen)
                return;
            Connect.IsEnabled = false;

        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            if (MyPort.IsOpen)
                MyPort.Close();
            Connect.IsEnabled = true;
        }

       /* private void Button_Click(object sender, RoutedEventArgs e)
        {

            SerialPort MyPort = new SerialPort("COM1",9600);//Adjust the comp port
            String[] portnames = SerialPort.GetPortNames();//get the port names
            try
            {
                if (MyPort.IsOpen == false)
                    
                    MyPort.Open();//opening the port
            }
            catch (System.ArgumentException)
            {
                Console.Write("Port not reachable");
            }
            



        }
//displaying of the ports in the textbox
        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            SerialPort import = new SerialPort();
            String[] portnames = SerialPort.GetPortNames();
            foreach(string port in SerialPort.GetPortNames())
            { portsdisplay.Text += port; }

        }*/
    }
}
