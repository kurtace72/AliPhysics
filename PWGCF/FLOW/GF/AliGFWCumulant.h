#ifndef ALIGFWCUMULANT__H
#define ALIGFWCUMULANT__H
#include "TComplex.h"
#include "TNamed.h"
#include "TMath.h"
#include "TAxis.h"
class AliGFWCumulant {
 public:
  AliGFWCumulant();
  ~AliGFWCumulant();
  void ResetQs();
  void FillArray(Double_t eta, Int_t ptin, Double_t phi, Double_t weight=1);
  enum UsedFlags_t {kBlank = 0, kFull=1, kPt=2};
  void SetType(UInt_t infl) { DestroyComplexVectorArray(); fUsed = infl; };
  void Inc() { fNEntries++; };
  Int_t GetN() { return fNEntries; };
  // protected:
  TComplex ***fQvector;
  UInt_t fUsed;
  Int_t fNEntries;
  //Q-vectors. Could be done recursively, but maybe defining each one of them explicitly is easier to read
  TComplex Vec(Int_t, Int_t, Int_t ptbin=0); //envelope class to summarize pt-dif. Q-vec getter
  Int_t fN; //! Harmonics
  Int_t fPow; //! Power
  Int_t fPt; //!fPt bins
  Bool_t fInitialized; //Arrays are initialized
  void CreateComplexVectorArray(Int_t N=1, Int_t P=1, Int_t Pt=1);
  void DestroyComplexVectorArray();
};

#endif
