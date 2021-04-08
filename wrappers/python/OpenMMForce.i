namespace OpenMM
{

class Force;

class Force {
public:
   virtual ~Force() ;

   int getForceGroup() const ;
   void setForceGroup(int group) ;
   virtual bool usesPeriodicBoundaryConditions() const ;
};

}
