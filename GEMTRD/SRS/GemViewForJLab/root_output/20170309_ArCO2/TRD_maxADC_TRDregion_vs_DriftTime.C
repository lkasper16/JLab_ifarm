{
//=========Macro generated from canvas: c_30a2f60_projection_12801/Canvas_1_n2
//=========  (Fri Mar 10 11:59:34 2017) by ROOT version5.34/21
   TCanvas *c_30a2f60_projection_12801 = new TCanvas("c_30a2f60_projection_12801", "Canvas_1_n2",27,132,834,595);
   gStyle->SetOptStat(0);
   c_30a2f60_projection_12801->Range(-40.71564,6861.234,531.0436,38083.7);
   c_30a2f60_projection_12801->SetFillColor(0);
   c_30a2f60_projection_12801->SetBorderMode(0);
   c_30a2f60_projection_12801->SetBorderSize(2);
   c_30a2f60_projection_12801->SetGridx();
   c_30a2f60_projection_12801->SetGridy();
   c_30a2f60_projection_12801->SetLeftMargin(0.1554217);
   c_30a2f60_projection_12801->SetRightMargin(0.04457831);
   c_30a2f60_projection_12801->SetFrameBorderMode(0);
   c_30a2f60_projection_12801->SetFrameBorderMode(0);
   
   TH1D *slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak = new TH1D("slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak","TRD Region: ADC peak vs. drift time",19,48.14815,505.5556);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(1,2854.594);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(2,14755.86);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(3,27660.63);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(4,31338.53);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(5,30535.83);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(6,28053.7);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(7,25668.75);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(8,23146.41);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(9,21541.75);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(10,20776.14);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(11,20307.1);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(12,20206.97);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(13,20103.9);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(14,20345.02);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(15,22109.88);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(16,25670.94);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(17,28971.29);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(18,20163.24);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(19,4208.837);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetMinimum(10000);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetMaximum(35000);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetEntries(408419);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetFillColor(30);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetLineColor(ci);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetXaxis()->SetTitle("drift time (ns)");
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetXaxis()->SetRange(1,19);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetXaxis()->SetLabelFont(42);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetXaxis()->SetTitleSize(0.05);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetXaxis()->SetTitleFont(42);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetYaxis()->SetTitle("Accumulated ADCs");
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetYaxis()->SetRange(1,1);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetYaxis()->SetLabelFont(42);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetYaxis()->SetTitleSize(0.05);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetYaxis()->SetTitleOffset(1.4);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetYaxis()->SetTitleFont(42);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetZaxis()->SetLabelFont(42);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetZaxis()->SetLabelSize(0.035);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetZaxis()->SetTitleSize(0.035);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->GetZaxis()->SetTitleFont(42);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->Draw("");
   
   TPaveText *pt = new TPaveText(0.1542169,0.9259259,0.9228916,0.989418,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("TRD Region: ADC peak vs. drift time");
   pt->Draw();
   c_30a2f60_projection_12801->Modified();
   c_30a2f60_projection_12801->cd();
   c_30a2f60_projection_12801->SetSelected(c_30a2f60_projection_12801);
}
