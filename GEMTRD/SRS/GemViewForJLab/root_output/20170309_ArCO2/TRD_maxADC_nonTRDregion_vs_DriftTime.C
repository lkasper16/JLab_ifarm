{
//=========Macro generated from canvas: c_30a2f60_projection_12801/Canvas_1_n2
//=========  (Fri Mar 10 11:50:04 2017) by ROOT version5.34/21
   TCanvas *c_30a2f60_projection_12801 = new TCanvas("c_30a2f60_projection_12801", "Canvas_1_n2",31,99,834,595);
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
   
   TH1D *slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak = new TH1D("slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak","Non TRD Region: ADC peak vs. drift time",19,48.14815,505.5556);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(1,1628.266);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(2,7500.159);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(3,16036.42);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(4,21931.01);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(5,23662.7);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(6,22528.8);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(7,20374.04);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(8,18389.07);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(9,16747.53);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(10,15666.84);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(11,15182.32);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(12,14885.39);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(13,14781.03);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(14,15359.14);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(15,17426.99);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(16,20534.92);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(17,21354.06);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(18,12022.16);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetBinContent(19,2271.316);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetMinimum(10000);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetMaximum(35000);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetEntries(298282);
   slice_px_of_TRDGEM1Xadc_vs_pos_timeBinPeak->SetFillColor(38);

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
   
   TPaveText *pt = new TPaveText(0.1668675,0.9135802,0.9439759,0.9964727,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Non TRD Region: ADC peak vs. drift time");
   pt->Draw();
   c_30a2f60_projection_12801->Modified();
   c_30a2f60_projection_12801->cd();
   c_30a2f60_projection_12801->SetSelected(c_30a2f60_projection_12801);
}
