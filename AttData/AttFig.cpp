void AttFig(Int_t step = 1500)
{
   ofstream fout("circle.att");

   Int_t t = 0;
   for(Int_t i = 0; i < 70; i++){
      if(i%7 == 0) t += step;
      fout << t << endl;
   }

   fout.close();
}
