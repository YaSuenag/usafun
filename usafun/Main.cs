/*
 * Copyright (C) 2015 Yasumasa Suenaga
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace usafun
{
    public partial class Main : Form
    {

        private const int MAX_PATH = 260;

        private string _tmpimage = null;

        public string tmpimage {

            get
            {
                return _tmpimage;
            }

            set
            {
                
                if((_tmpimage != null) && (_tmpimage != value))
                {
                    File.Delete(_tmpimage);
                }

                _tmpimage = value;
            }

        }

        private Configuration configForm;

        [DllImport("imgproc.dll")]
        public static extern int FindFecals(String fname, StringBuilder tmpfname, double bin_thresh, float roi_percentage,
                                                                                double skip_area_sz, float min_cluster_ratio);

        public Main()
        {
            InitializeComponent();

            configForm = new Configuration();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if(openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                label1.Text = openFileDialog1.FileName;
                pictureBox1.ImageLocation = label1.Text;
                button2.Enabled = true;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            StringBuilder tmpfname = new StringBuilder(MAX_PATH);

            int fecals = FindFecals(label1.Text, tmpfname, configForm.bin_thresh, configForm.roi_percentage,
                                                            configForm.skip_area_sz, configForm.min_cluster_ratio);
            MessageBox.Show("糞 = " + fecals.ToString());

            tmpimage = tmpfname.ToString();
            pictureBox1.ImageLocation = tmpimage;
        }

        private void Main_FormClosed(object sender, FormClosedEventArgs e)
        {
            if(tmpimage != null)
            {
                File.Delete(tmpimage);
            }

            configForm.Dispose();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            configForm.ShowDialog();
        }
    }
}
