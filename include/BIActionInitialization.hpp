#ifndef BIActionInitialization_h
#define BIActionInitialization_h 1

#include "G4VUserActionInitialization.hh"


class BIActionInitialization : public G4VUserActionInitialization
{
public:
   BIActionInitialization();
   virtual ~BIActionInitialization();

   virtual void BuildForMaster() const;
   virtual void Build() const;

private:

};

#endif
