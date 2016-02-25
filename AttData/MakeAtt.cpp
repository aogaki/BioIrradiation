void MakeAtt(Int_t t = 0)
{
   ofstream fout(Form("att%d.dat", t));
   for(Int_t y = 0; y < 12; y++){
      fout << t <<" "<< t <<" "<< t <<" "<< t <<" "<< t <<" "<< t <<" "<< t <<" "<< t << endl;
   }
}
