using FinalProject_Winform.Data;
using FinalProject_Winform.Models.domain;

namespace FinalProject_Winform
{
    public partial class LoginForm : Form
    {
        private readonly FinalDbContext db;
        TextBox idbox;
        TextBox pwbox;
        public LoginForm()
        {
            InitializeComponent();
            idbox = textBox1;
            pwbox = textBox2;
            db = new FinalDbContext();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            var login = Login(idbox.Text, pwbox.Text);
            if (login != null)
            {
                MainForm mainForm = new MainForm();
                mainForm.Show();
                this.Hide();
            }
            else
            {
                MessageBox.Show("�α��� ����\n�Է��Ͻ� ������ �ٽ� Ȯ�����ּ���.");
            }

        }

        private User Login(string id, string pw)
        {
            var a = db.Users.FirstOrDefault(x => x.PUser_id == id && x.PUser_password == pw);
            if (a == null) return null;
            return a;
        }

        private void label4_Click(object sender, EventArgs e)
        {
            AddUserForm addUserForm = new AddUserForm();
            addUserForm.Show();
        }

        private void textBox2_KeyPress(object sender, KeyPressEventArgs e)
        {
            if(e.KeyChar == 13)
            {
                var login = Login(idbox.Text, pwbox.Text);
                if (login != null)
                {
                    MainForm mainForm = new MainForm();
                    mainForm.Show();
                    this.Hide();
                }
                else
                {
                    MessageBox.Show("�α��� ����\n�Է��Ͻ� ������ �ٽ� Ȯ�����ּ���.");
                    MessageBox.Show("dd");
                }
            }
        }
    }
    //