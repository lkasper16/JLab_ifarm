{
//=========Macro generated from canvas: c_3bcb840_projection_12801/Canvas_1_n3
//=========  (Fri Mar 10 17:29:54 2017) by ROOT version5.34/21
   TCanvas *c_3bcb840_projection_12801 = new TCanvas("c_3bcb840_projection_12801", "Canvas_1_n3",11,67,829,661);
   gStyle->SetOptStat(0);
   c_3bcb840_projection_12801->Range(-28.08642,79833.33,543.6728,180000);
   c_3bcb840_projection_12801->SetFillColor(0);
   c_3bcb840_projection_12801->SetBorderMode(0);
   c_3bcb840_projection_12801->SetBorderSize(2);
   c_3bcb840_projection_12801->SetGridx();
   c_3bcb840_projection_12801->SetGridy();
   c_3bcb840_projection_12801->SetLeftMargin(0.1333333);
   c_3bcb840_projection_12801->SetRightMargin(0.06666667);
   c_3bcb840_projection_12801->SetFrameBorderMode(0);
   c_3bcb840_projection_12801->SetFrameBorderMode(0);
   
   TH1D *slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin = new TH1D("slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin","Non TRD region: Integrated ADCs vs.drift time",19,48.14815,505.5556);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(1,48820.7);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(2,73545.03);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(3,98537.76);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(4,117614.4);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(5,129275.3);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(6,134961.1);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(7,136524.7);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(8,135774.4);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(9,133701.4);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(10,131480.5);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(11,129392.8);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(12,127637.4);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(13,126758.7);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(14,126645);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(15,127159.5);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(16,127205.7);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(17,122139.7);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(18,108439.2);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetBinContent(19,90125.99);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetMinimum(90000);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetMaximum(170000);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetEntries(2225739);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetStats(0);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetFillColor(38);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->SetLineColor(ci);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->GetXaxis()->SetTitle("drift time (ns)");
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->GetXaxis()->SetRange(1,19);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->GetXaxis()->SetLabelFont(42);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->GetXaxis()->SetTitleSize(0.05);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->GetXaxis()->SetTitleFont(42);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->GetYaxis()->SetTitle("Accumulated ADCs");
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->GetYaxis()->SetLabelFont(42);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->GetYaxis()->SetLabelSize(0.035);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->GetYaxis()->SetTitleSize(0.05);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->GetYaxis()->SetTitleFont(42);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->GetZaxis()->SetLabelFont(42);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->GetZaxis()->SetLabelSize(0.035);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->GetZaxis()->SetTitleSize(0.035);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->GetZaxis()->SetTitleFont(42);
   slice_px_of_TRDGEM1Xadc_vs_pos_allTimeBin->Draw("");
   
   TPaveText *pt = new TPaveText(0.1490909,0.92891,0.849697,0.9905213,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Non TRD region: Integrated ADCs vs.drift time");
   pt->Draw();
   c_3bcb840_projection_12801->Modified();
   c_3bcb840_projection_12801->cd();
   c_3bcb840_projection_12801->SetSelected(c_3bcb840_projection_12801);
}
