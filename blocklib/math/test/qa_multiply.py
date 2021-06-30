#!/usr/bin/env python3
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#
#


from newsched import gr, gr_unittest, math, blocks
# import pmt


class test_multiply(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.flowgraph()

    def tearDown(self):
        self.tb = None

    def test_multiply_ss(self):
        tb = self.tb
        src_data1 = [x for x in range(0, 100)]
        src_data2 = [x for x in range(0, 100)]
        expected_data = [src_data1[x] * src_data2[x] for x in range(0, 100)]

        src1 = blocks.vector_source_s(src_data1)
        src2 = blocks.vector_source_s(src_data2)
        op = math.multiply_ss()
        dst = blocks.vector_sink_s()

        tb.connect(src1,0, op,0)
        tb.connect(src2,0, op,1)
        tb.connect(op, dst)
        tb.run()
        dst_data = dst.data()
        self.assertEqual(expected_data, dst_data)

    def test_multiply_ii(self):
        tb = self.tb
        src_data1 = [x for x in range(0, 100)]
        src_data2 = [x for x in range(0, 100)]
        expected_data = [src_data1[x] * src_data2[x] for x in range(0, 100)]

        src1 = blocks.vector_source_i(src_data1)
        src2 = blocks.vector_source_i(src_data2)
        op = math.multiply_ii()
        dst = blocks.vector_sink_i()

        tb.connect(src1,0, op,0)
        tb.connect(src2,0, op,1)
        tb.connect(op, dst)
        tb.run()
        dst_data = dst.data()
        self.assertEqual(expected_data, dst_data)

    def test_multiply_ff(self):
        tb = self.tb
        src_data1 = [float(x) for x in range(0, 100)]
        src_data2 = [float(x) for x in range(0, 100)]
        expected_data = [src_data1[x] * src_data2[x] for x in range(0, 100)]

        src1 = blocks.vector_source_f(src_data1)
        src2 = blocks.vector_source_f(src_data2)
        op = math.multiply_ff()
        dst = blocks.vector_sink_f()

        tb.connect(src1,0, op,0)
        tb.connect(src2,0, op,1)
        tb.connect(op, dst)
        tb.run()
        dst_data = dst.data()
        for exp, dst in zip(expected_data, dst_data):
            self.assertAlmostEqual(exp, dst)

    def test_multiply_cc(self):
        tb = self.tb
        src_data1 = [complex(x,-x) for x in range(0, 100)]
        src_data2 = [complex(x,-x) for x in range(0, 100)]
        expected_data = [src_data1[x] * src_data2[x] for x in range(0, 100)]
        
        src1 = blocks.vector_source_c(src_data1)
        src2 = blocks.vector_source_c(src_data2)
        op = math.multiply_cc()
        dst = blocks.vector_sink_c()

        tb.connect(src1,0, op,0)
        tb.connect(src2,0, op,1)
        tb.connect(op, dst)
        tb.run()
        dst_data = dst.data()
        for exp, dst in zip(expected_data, dst_data):
            self.assertAlmostEqual(exp, dst)


if __name__ == '__main__':
    gr_unittest.run(test_multiply)