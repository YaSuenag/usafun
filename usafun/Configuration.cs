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
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace usafun
{
    public partial class Configuration : Form
    {

        public double bin_thresh { get; set; } = 45.0;

        public float roi_percentage { get; set; } = 95.0f;

        public double skip_area_sz { get; set; } = 100.0;

        public float min_cluster_ratio { get; set; } = 2.5f;

        public Configuration()
        {
            InitializeComponent();

        }

        private void Configuration_FormClosing(object sender, FormClosingEventArgs e)
        {
            bin_thresh = double.Parse(textBox1.Text);
            roi_percentage = float.Parse(textBox2.Text);
            skip_area_sz = double.Parse(textBox3.Text);
            min_cluster_ratio = float.Parse(textBox4.Text);
        }

        private void Configuration_Shown(object sender, EventArgs e)
        {
            textBox1.Text = bin_thresh.ToString();
            textBox2.Text = roi_percentage.ToString();
            textBox3.Text = skip_area_sz.ToString();
            textBox4.Text = min_cluster_ratio.ToString();
        }
    }
}
