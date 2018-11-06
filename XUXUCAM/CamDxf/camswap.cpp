#include "CamDxf/camswap.h"

Chromosome *parent1;
Chromosome *parent2;
Chromosome *mutated;
Chromosome parentOne;
Chromosome parentTwo;
Chromosome child;
/* given a set of points we create a population*/
///@note Are those set by the compiler as STATIC ????
int nbrElite1=0;
int nbrElite2=0;
int nbimporte=0;
/// to follow the created generations number
int curGen=0;
/// mutation rate is given in %
int mutRate=20;
/// mutation number = Pop size/ mutation rate
int mutNbr;

Popu::Popu(){
         totalRoute=0;
         popSize=20;
         Max_iter=13;
         chromoSize=1;
         mutNbr=((popSize/100)*mutRate);
         //qDebug()<<mutNbr;
         if (mutNbr==0) {mutNbr=1;}
         //mutNbr = (mutNbr=0) ? 1 : mutNbr;//if (mutNbr==0
         //qDebug()<<mutNbr;
         mutNbr=2;
     }


QPFWPVector Popu::init(QPFWPVector points,bool randomtartupPop){
         Chromosome initialRoute;
         initialRoute.setElements(points,false);
         qDebug()<<"Without optimizationthe route length is"<<initialRoute.routeLength;

         int i=0;
         chromoSize=points.size();
         // qDebug()<<"Creating initial Rpop";
         //create a population of @see popSize chromosome. The chromosome elements are taken randomly from parent @see points
         while (i<popSize){
                 Chromosome member;
                 member.setElements(points,randomtartupPop);
                 content.append(member);
                 //fixme: need to be computed only once
                 totalRoute=member.routeLength;
         i++;
        }

        sortContent();
        // copy the created population into newGen
        newGen=content;
        int sameVal=0;
        double lastRoute=totalRoute;
        // Keep creating new genrations until @see Max_iter or we stay in a local optimum fo n times
        for (curGen=0;curGen<Max_iter && sameVal< 3;curGen++){
                 createNewGen();
                 if (lastRoute==content[0].getRouteLength()){
                     sameVal++;
                 }
                 else{
                     lastRoute=content[0].getRouteLength();
                     sameVal=0;
                 }
                 //qDebug()<<sameVal;
                 //qDebug()<< content[0].getRouteLength();
        }

        return content[0].elements;
     }

void Chromosome::setElements(QPFWPVector points,int size){
         int i=0;
         while (i<size){
                 elements<<points.at(i);
                 i++;
                }
         }
 Chromosome Popu::crossOver(Chromosome parent1, Chromosome parent2){
             int pos=3,chromoSize=parent2.elements.size()-1;
             Chromosome child;
             int childSize=0;

             pos=qrand()%chromoSize;
             // check that we can make crossover
             if (pos<=2 && chromoSize>2) pos=2;
             child.setElements(parent1.elements,pos);
             for (int i=1; i< (chromoSize-pos) ; i++) {
                 if (!child.elements.contains(parent2.elements.at(pos+i))) {
                     child.elements<<parent2.elements.at(pos+i);
                 }
             }
             /// now we have to complete the route thus to make a valid one
             childSize=child.elements.size();
             for (int i=0; i<=chromoSize ; i++) {
                 if (!child.elements.contains(parent2.elements.at(i))) {
                     child.elements.append(parent2.elements.at(i));
                 }
             }
             return child;
         }
void Popu::sortContent(){
        for(int i = popSize - 1; i > 0; i--) {
            for(int j = 0; j < i; j++){
                if(content[j].fitness > content[j + 1].fitness){
                Chromosome chromosome = content[j + 1];
                content[j + 1] = content[j];
                content[j] = chromosome;
                }
            }
        }
        // qDebug()<<"Best generation"<< curGen <<"length:"<<content[0].routeLength;
        totalRoute=content[0].routeLength;
    }

/// @todo The only thing we nedd from QPOintWithParent is the ParentLoop!!Imporvo this by overloading it
/// @todo Have to stor all the compiuted disantce and to reuse them when dneeded

/**
 *
 * @param p1 First point
 * @param p2 second point
 * @return Cartesian distance between p1 and p2
 */
qreal dist(QPointFWithParent p1,QPointFWithParent p2) {
     return sqrt(((p1.x()-p2.x())*(p1.x()-p2.x()))+((p1.y()-p2.y())*(p1.y()-p2.y())));
    }


void swapVals(QPFWPVector &vect,int pos1,int pos2){
     QPointFWithParent temp=vect[pos2];
     vect[pos2]=vect[pos1];
     vect[pos1]=temp;
    }



 void Popu::createNewGen(){
     //Chromosome child;
     ///Apply Ellitism by keeping the 2 first chromosomes
     newGen[0]=content[0];
     newGen[1]=content[1];
     //qDebug()<<"Ellistism performed"<<content[0].fitness<<content[1].fitness;
     /// All parents expect the elites are mutated
     // child=crossOver( content[0],content[1]);
     // child.calcRouteLength();
     //qDebug()<<"Created an elite offspring with route length"<<child.routeLength;
     //mutateImproveMe(0);
     // mutateImproveOffspring(child, 0);
     for (int i=2; i<popSize;i++){
     /// mutation is done at given rate
     //child=crossOver( content[i-1],content[i]);
     //child.calcRouteLength();
     //qDebug()<<"Created an offspring with route length"<<child.routeLength;
     mutateImproveMe(i);
     //mutateImproveOffspring(child, i);
     // }
     }
     //qDebug()<<"New generation created";
     content=newGen;
     sortContent();
    }


void Popu::mutateImproveOffspring(Chromosome &toImprove, int pos){

        int orignalRouteLength=toImprove.routeLength;
        int pos1=1;
        int pos2=2;
        int oldRouteLength=orignalRouteLength;
        for (int j=0;j<chromoSize;j++){
            pos1=qrand()%chromoSize;
            pos2=qrand()%chromoSize;
            while (pos2==pos1){
            pos2=qrand()%chromoSize;
            }
            oldRouteLength=toImprove.routeLength;
            swapVals(toImprove.elements,pos1,pos2);
            toImprove.calcRouteLength();

        if ( toImprove.routeLength>oldRouteLength){
            swapVals(toImprove.elements,pos1,pos2);
            toImprove.calcRouteLength();// could be simplier =oldRouteLength ?
           }
        }
        newGen.replace(pos,toImprove);
     }

void Popu::mutateImproveMe(int pos){
        Chromosome toImprove;
                //we mutate beginig from an elite parent
                if (qrand()%3==0) {
                toImprove=content[0];
                nbrElite1++;
                }
              // We mutate a random chromosome
                else if (qrand()%3==1){
                toImprove=content.at(pos);
                nbimporte++;
            }
                else {
                toImprove=content[1];
                nbrElite2++;
                }

        int orignalRouteLength=toImprove.routeLength;
        int pos1=1;
        int pos2=2;
        int oldRouteLength=orignalRouteLength;
        for (int j=0;j<chromoSize;j++){
            pos1=qrand()%chromoSize;
            pos2=qrand()%chromoSize;
            while (pos2==pos1){
            pos2=qrand()%chromoSize;
            }
            oldRouteLength=toImprove.routeLength;
            swapVals(toImprove.elements,pos1,pos2);
            toImprove.calcRouteLength();

                        if ( toImprove.routeLength>oldRouteLength){
                                swapVals(toImprove.elements,pos1,pos2);
                                toImprove.calcRouteLength();// could be simplier =oldRouteLength ?
                        }
        }
     newGen.replace(pos,toImprove);
     }


void Popu::mutateImprove(int pos){

      Chromosome toImprove;
         // Time to improve the route (randomly for now) even the elite are concerned
         for (int i=0;i<2;i++){
             if (i==0)
                toImprove=parentOne;
            else
                toImprove=parentTwo;
            //qDebug()<<"Trying to improve parent n°"<<pos+i;
            //FOR debug remove me!
             int orignalRouteLength=toImprove.routeLength;
                for (int j=0;j<chromoSize*2;j++){
                 int pos1=qrand()%chromoSize;
                 int pos2=qrand()%chromoSize;
                 int oldRouteLength=toImprove.routeLength;
                 //toImprove.elements.swap(pos1,pos2);
                 swapVals(toImprove.elements,pos1,pos2);
                 toImprove.calcRouteLength();
                // qDebug()<<toImprove.routeLength<<oldRouteLength;
                 //undo changes as this become worse than before
                 if ( toImprove.routeLength>oldRouteLength){
                     //toImprove.elements.swap(pos1,pos2);
                     swapVals(toImprove.elements,pos1,pos2);
                     toImprove.calcRouteLength();/// could be simplier =oldRouteLength ?
                     //qDebug()<<"no changes";
                    }
                    //else{
                     //keep changes
                    //}
                }
                newGen.replace(i+pos,toImprove);
                //newGen.replace(pos,toImprove);
                //qDebug()<<"Improved*********"<<toImprove.routeLength<<orignalRouteLength;
                //qDebug()<<toImprove.elements;
                 //qDebug()<<"improved mutant routes"<< toImprove.routeLength;

            }
     }

     void Popu::selectTwoParents(Chromosome* p1,Chromosome* p2,int posd){
     //We have to start from 2 to skip elite parents
     int k=posd;
     int bestfitPos=2,bestfitOldPos=2;
     double bestfit=10000,bestfitOld=10000,tempfit=0;
     Chromosome mem;
     *p1=content.at(posd);
     *p2=content.at(posd+1);
     return ;
    }


     ///@todo: ameliortae parent selection process
 void Popu::selectParents(Chromosome* p1,Chromosome* p2,int posd){
     //We have to start from 2 to skip elite parents
     int k=posd;
     int bestfitPos=2,bestfitOldPos=2;
     double bestfit=10000,bestfitOld=10000,tempfit=0;
     Chromosome mem;
     ///while (k<content.size()){
        /// mem=content.at(k);
        /// tempfit=(mem.routeLength);//)/totalRoute;
         //qDebug()<<k<<"-fitness :"<<tempfit<<"route :"<<mem.fitness;
        /// mem.setFitness(tempfit);
         ///Fixme: avoid the case best fitness encountred at first pos
         ///Note: must be <= to avoid keeping 10000 as fitness if equal although equal fitneess is not desired
        /// if (tempfit<bestfit) {
        ///	 bestfitOld=bestfit;
        ///	 bestfit=tempfit;
        ///	 bestfitOldPos=bestfitPos;
        ///	 bestfitPos=k;
        ///	 }
        ///	 if (tempfit<bestfitOld && tempfit!=bestfit) {bestfitOld=tempfit;}
        /// k++;
        ///}

        ///FIXME for now we're taking parents ordered
     ///*p1=content.at(bestfitPos);
     ///*p2=content.at(bestfitOldPos);
     *p1=content.at(posd);
     *p2=content.at(posd+1);
     /// qDebug()<<"Selectd parents "<<bestfitPos<<" and "<<bestfitOldPos<<p1->elements<<p2->elements;
     return ;
    }




    void Popu::computeTotalRoute(){



    }


void Popu::mutate(int pos){

     int pos1=0,pos2=3;//size=offspring.elements.size();
      //QTime t;
      int rd=3;
      Chromosome mutant;

     for (int i=0;i<2;i++){
         if (i==0)
                mutant=parentOne;
            else
                mutant=parentTwo;

         for (int j=0;j<chromoSize;j++){
             ///@todo make it dependant on a variable value Mut_prob the probability should be small
             rd=qrand()%chromoSize;
             if (rd >chromoSize-(chromoSize/5)) {
                 //qDebug()<<"creating a mutant";
                 pos1=qrand()%chromoSize;
                 //qsrand(QTime::msec ());
                 pos2=qrand()%chromoSize;
                 //qDebug()<<pos2<<pos1;
                 swapVals(mutant.elements,pos1,pos2);
                }
             //qDebug()<<mutant.elements<<"route"<< mutant.routeLength;
            }
         mutant.calcRouteLength();
         //qDebug()<<"creating a mutant at "<<pos+i<<"routes"<< mutant.routeLength;
    // @NOTE As we kept the 2 best parents as are (ellitisme) and i>0 at while loop end we keep the elite
         //newGen.replace(pos+i,mutant);
        newGen.replace(pos,mutant);
        //qDebug()<<newGen.size();
        }
    }

     /// returns an offspring from 2 parents
    Chromosome Popu::cross(Chromosome parent1, Chromosome parent2){

     int i=0,pos=0,posOther=0,sizeFixe=parent2.elements.size()-1,size=parent2.elements.size()-1;
     double dist1=0,dist2=0;

     QPointFWithParent currentVal=parent1.elements.at(0);
     QPointFWithParent otherVal=parent2.elements.at(1),nextVal=parent1.elements.at(1);
     // the other chromosome when working with p1 it's p2 and vice versa
     Chromosome activeParent=parent1;
     Chromosome otherParent=parent2;
     int activeParentRef=1;
     // this reference chromosem is used when need to insert a random elment, as we delete from it every
     // elmenet we append to the offsprings, we're sure we don't append duplicate points
     Chromosome referenceParent;
     referenceParent.elements=parent2.elements;/// could be parent1 also
     Chromosome offspring;
     //qDebug()<<"creating an offspring of "<<size+1<<"elements";

     for (int i=0;i< sizeFixe-2;i++){
         posOther=otherParent.elements.indexOf(currentVal);
         pos=activeParent.elements.indexOf(currentVal);
         /*
         //check for case at list end => start at a random pos if so
        if (posOther==size ){
             //TODO:try with the other point before giving up
                if (referenceParent.elements.size()!=0){
                 int j=0;
                 while (offspring.elements.contains(referenceParent.elements.at(j))){
               posOther=otherParent.elements.indexOf(referenceParent.elements.at(j));
                qDebug()<<"looping back randomly";j++;
             }
                 qDebug()<<"stopped at" <<j;
                }
            }
         if (pos==size) {
            if (referenceParent.elements.size()!=0){
             int k=0;
                 while (offspring.elements.contains(referenceParent.elements.at(k))){
                 posOther=otherParent.elements.indexOf(referenceParent.elements.at(k));
                 qDebug()<<"looping back randomly";k++;///i++;
                 }
                 qDebug()<<"stopped at" <<k;
                currentVal=referenceParent->elements.takeAt(0);
            }
        }
          qDebug()<<"We're in pos="<<pos<<" Other parent contains"<<currentVal<<" at pos"<<posOther<<"size="<<size;
         /recheck if the random city is at list end : this tile we have to go to list begin **/
         if (posOther==size || posOther>size) {
             //TODO:try with the other point before giving up
             posOther=0;//qDebug()<<"have to loop back";
            }
        if (pos==size || pos>size){
             pos=0;//qDebug()<<"have to loop back";
            }
         // get the next 'city' coordinates to compute how far is it
         otherVal=otherParent.elements.at(posOther+1);
          //qDebug()<<"deciding wich paretn to keep with";
          //qDebug()<<activeParent->elements;
          nextVal=activeParent.elements.at(pos+1);
          // the distance from the other parent cuurent point to next point in it
          dist2=dist(otherVal,currentVal);
          // the distance from curretn parent to next point in active parent
          dist1=dist(nextVal,currentVal);

         // if dist1<dist2 have to copy the path from the other parent and make it the active one;
         // UNLES it's already prensent in the offspring
         offspring.elements.append(currentVal);
         referenceParent.elements.remove(referenceParent.elements.indexOf(currentVal));
          parent1.elements.remove(parent1.elements.indexOf(currentVal));
           parent2.elements.remove(parent2.elements.indexOf(currentVal));
           size=parent2.elements.size()-1;
         //qDebug()<<parent2.elements<<parent1.elements;
         if (dist2 < dist1) {
            //qDebug()<<"switching to other parent";
             if (offspring.elements.contains(otherVal)){
                 qDebug()<<"Point already presnet: forcing the other";
                 if (!offspring.elements.contains(nextVal)){
                     activeParent=parent2;otherParent=parent1;activeParentRef=2;
                     currentVal=nextVal;
                     continue;
                     }
                 // use a random take
                 if (referenceParent.elements.size()!=0){
                     qDebug()<<"Everything failed :setting a random point";

                     currentVal=referenceParent.elements.at(0);
                     referenceParent.elements.remove(0);
                    }
                 else{
                 //fixme
                     qDebug()<<"Nothing to take from reference!!??";
                     break;
                    }
                    //i++;///time to
                 continue;

                }
             // change the active parent
             if (activeParentRef==2){
             activeParent=parent2;otherParent=parent1;activeParentRef=2;
             }
             else{
             activeParent=parent1;otherParent=parent2;activeParentRef=1;
             }
             // update current val
             currentVal=otherVal;
            }
            // continue with the current parent
         else{
             //qDebug()<<"Continuing with this parent";
             //offspring->elements.append(currentVal);
             //offspring->elements.append(nextVal);
             //keeping the parent
             //activeParent=parent1;otherParent=parent2;
             // update current val
             currentVal=nextVal;
            }
            //qDebug()<<offspring.elements;


        }

     //offspring->elements.append(currentVal);
     // finally add the only point that remain in the ref chromosome
     //if (referenceParent.elements.size()!=0){
     offspring.elements.append(referenceParent.elements.at(0));
     referenceParent.elements.remove(0);
     offspring.elements.append(referenceParent.elements.at(0));
     referenceParent.elements.remove(0);
     //}
     offspring.calcRouteLength();
     return offspring;
    }

//Compute the chromosome tour length
void Chromosome::calcRouteLength(){
     routeLength=0;//qDebug()<<elements;
     for (int i=0;i<elements.size()-1;i++){
         //if (i%2==1){
         //qDebug()<<routeLength<<elements.at(i)<<elements.at(i+1);
             routeLength+=dist(elements.at(i),elements.at(i+1));
           // }
        }
     fitness=routeLength;
     return ;
    }



void Chromosome::setElements(QPFWPVector points,bool random){
     int pos=0,i=0,size=points.size();
     QPFWPVector newChrom;
     MySize=points.size();
     //newChrom.clear();

     while (i<MySize){
         size=points.size();
         ///@note: to prevent takin pos out of list range we update size every time we take a value
         if (random){
             pos=qrand()%size;
         }
         else{
                         pos=0;
         }

         newChrom.append(points.at(pos));
         points.remove(pos);
         //if (i%2==1) { /// time to compute the distance
         /// @todo: manage to store the distances matrix to recal it when neede instead of recomputing the values every time we need them
            // routeLength+=dist(newChrom.at(i),newChrom.at(i-1));
            //}
         i++;
        }
         elements =newChrom;
        calcRouteLength();
     //setFitness(routeLength);
     //qDebug()<<"route length"<<routeLength;

     }


Population::Population(QPFWPVector points){
     int i=0,totalRoute=0;popSize=20;
     int chromoSize=points.size();
     Chromosome *offspring=new Chromosome(); mutated=new Chromosome();
     QVector <Chromosome *> newContent;
     while (i<=popSize){
         Chromosome *member=new Chromosome();
         //qDebug()<<"Adding  the "<<i<<"chromo";
         member->setElements(points,true);
         content.append(member);
         totalRoute+=member->routeLength;
         i++;
        }
     qDebug()<<"the pop total route is"<<totalRoute;
     /// apply greedy crossover
     /// content.clear();
     int nbr_it=0;
     i=0;int j=0;
     //while (j<popSize){

         while (i<2 && nbr_it<popSize+100){
             selectElite();
             qDebug()<<"Selecting Elite n°:"<<i;


             offspring=crossover(parent1,parent2);

                for (j=0;j<offspring->elements.size();j++){
     if (j%2==1){
     offspring->routeLength+=dist(offspring->elements.at(j),offspring->elements.at(j-1));
     }
     }
     qDebug()<<"new route offspring:"<<offspring->routeLength;
             /// start mutation to genrate new population:
             mutate(offspring);
         ///reject trisomic ones :)
         /// if (offspring->elements.size()==chromoSize){
             ///newContent.append(offspring);
         /// content.append(offspring);
            qDebug()<<"Generating population n°:"<<i;
             i++;
         ///	}
         ///else {
            /// qDebug()<<"rejecting trisomiic one";
            ///}
            ///nbr_it++;
            }
         //content=newContent;
         //j++;
     ///qDebug()<<content.size();
        //}
    }
    /// todo: create a function that return the pop/chromosome size for brievty
    void Population::mutate(Chromosome *offspring){
     /// given an offspring we mutate it at a certain rate and create the new population
     int i=0,pos1=0,pos2=3,size=offspring->elements.size();content.clear();

     for (i=0;i<20;i++){
         mutated->elements=offspring->elements;
         if (i%3==0) {
         pos1=qrand()%size;
         pos2=qrand()%size;
         //qDebug()<<"swapping values at"<<pos1 <<"and"<<pos2 ;
         //mutated->elements.swap(pos1,pos2);
          swapVals(mutated->elements,pos1,pos2);
         }
          ///qDebug()<<mutated->elements;

         content.append(mutated);
        }
        qDebug()<<content;
    }


void Population::selectElite(){
     ///select 2 parents
     int i=0, bestfitPos=0,bestfitOldPos=0;
     double bestfit=10000,bestfitOld=10000,tempfit=0;
     while (i<content.size()){
         tempfit=(content.at(i)->routeLength);///totalRoute
         qDebug()<<i<<"fitness is"<<tempfit<<"route is "<<content.at(i)->routeLength;
         content.at(i)->fitness=tempfit;
         ///Fixme: avoid the case best fitness encountred at first pos
         if (tempfit<bestfit) {
             bestfitOld=bestfit;
             bestfit=tempfit;
             bestfitOldPos=bestfitPos;
             bestfitPos=i;
             }
             if (tempfit<bestfitOld && tempfit!=bestfit) {bestfitOld=tempfit;}
         //qDebug()<<"Chromo  "<<i<<"fitness="<<content.at(i)->fitness;
         i++;
        }
     parent1=content.at(bestfitPos);
     parent2=content.at(bestfitOldPos);

     qDebug()<<"Taking 2 parents with fitness:"<<bestfit<<bestfitOld;

     return ;
     }


     /// returns an offspring from 2 parents
     Chromosome *Population::crossover(Chromosome *parent1, Chromosome *parent2){
     qDebug()<<parent1->elements;
     qDebug()<<parent2->elements;
     int i=0,pos=0,posOther=0,size=parent2->elements.size()-1;
     double dist1=0,dist2=0;
     QPointFWithParent currentVal=parent1->elements.at(0);
     QPointFWithParent otherVal=parent1->elements.at(1),nextVal=parent1->elements.at(1);
     /// the other chromosome when working with p1 it's p2 and vice versa
     Chromosome *activeParent=parent1;
     Chromosome *otherParent=parent2;
     /// this reference chromosem is used when need to insert a random elment, as we delete from it every
     /// elmenet we append to the offsprings, we're sure we don't append duplicate points
     Chromosome *referenceParent=new Chromosome();
     referenceParent->elements=parent2->elements;/// could be parent1 also
     Chromosome *offspring=new Chromosome();
     qDebug()<<"creating an offspring of "<<size+1<<"elements";

     while (i< size){
         /// fixme : shouold we check for multiple points first?

         posOther=otherParent->elements.indexOf(currentVal);
         pos=activeParent->elements.indexOf(currentVal);
         ///check for case at list end => start at a random pos if so
         if (posOther==size ){
             ///TODO:try with the other point before giving up
                if (referenceParent->elements.size()!=0){
                 int j=0;
                 while (offspring->elements.contains(referenceParent->elements.at(j))){
                 posOther=otherParent->elements.indexOf(referenceParent->elements.at(j));
                 qDebug()<<"looping back randomly";j++;
                 ///fixme
                 ///i++;
                 }
                 qDebug()<<"stopped at" <<j;
                }
            }
         if (pos==size) {
            if (referenceParent->elements.size()!=0){
             int k=0;
                 while (offspring->elements.contains(referenceParent->elements.at(k))){
                 posOther=otherParent->elements.indexOf(referenceParent->elements.at(k));
                 qDebug()<<"looping back randomly";k++;///i++;
                 }
                 qDebug()<<"stopped at" <<k;
                //currentVal=referenceParent->elements.takeAt(0);
            }
        }
         qDebug()<<"We're in pos="<<pos<<" Other parent contains"<<currentVal<<" at pos"<<posOther;
        /// recheck if the random city is at list end : this tile we have to go to list begin
         if (posOther==size) {
             ///TODO:try with the other point before giving up
             posOther=0;qDebug()<<"have to loop back";
            }
        if (pos==size){
             pos=0;qDebug()<<"have to loop back";
            }
         /// get the next 'city' coordinates to compute how far is it
         otherVal=otherParent->elements.at(posOther+1);
          //qDebug()<<"deciding wich paretn to keep with";
          //qDebug()<<activeParent->elements;
          nextVal=activeParent->elements.at(pos+1);
          /// the distance from the other parent cuurent point to next point in it
          dist2=dist(otherVal,currentVal);
          /// the distance from curretn parent to next point in active parent
          dist1=dist(nextVal,currentVal);

         /// if dist1<dist2 have to copy the path from the other parent and make it the active one;
         /// UNLES it's already prensent in the offspring
         offspring->elements.append(currentVal);
         referenceParent->elements.remove(referenceParent->elements.indexOf(currentVal));
         //qDebug()<<referenceParent->elements;
         if (dist2 < dist1) {
             qDebug()<<"switching to other parent";
             if (offspring->elements.contains(otherVal)){
                 qDebug()<<"Point already presnet: forcing the other";
                 if (!offspring->elements.contains(nextVal)){
                     currentVal=nextVal;
                     continue;
                     }
                 /// use a random take
                 if (referenceParent->elements.size()!=0){
                     qDebug()<<"Everything failed :setting a random point";
                     currentVal=referenceParent->elements.at(0);
                     referenceParent->elements.remove(0);
                    }
                 else{
                 ///fixme
                     qDebug()<<"Nothing to take from reference!!??";
                     break;
                    }
                    ///i++;///time to
                 continue;

                }
             /// change the active parent
             if (activeParent==parent1){
             activeParent=parent2;otherParent=parent1;
             }
             else{
             activeParent=parent1;otherParent=parent2;
             }
             /// update current val
             currentVal=otherVal;
            }
            /// continue with the current parent
         else{
             qDebug()<<"Continuing with this parent";
             //offspring->elements.append(currentVal);
             ///offspring->elements.append(nextVal);
             ///keeping the parent
             //activeParent=parent1;otherParent=parent2;
             /// update current val
             currentVal=nextVal;
            }
            qDebug()<<offspring->elements;
         i++;

        }

     ///offspring->elements.append(currentVal);
     /// finally add the only point that remain in the ref chromosome
     if (referenceParent->elements.size()!=0){
     offspring->elements.append(referenceParent->elements.at(0));
     referenceParent->elements.remove(0);
     }
     qDebug()<<offspring->elements;
     return offspring;
    }


    /** Select two elite parents*/
    //Population::Select(){
     //Chromosome parent1= roulette(this,totalRoute);
     //Chromosome parent2= roulette(this,totalRoute);

    //}

/**A chromosome is randomly generated from a given set of points*/


Chromosome::Chromosome(){
     routeLength=0;
     fitness=0;
     MySize=1;
    }




Population::~Population(){


}



QPFWPVector  Chromosome::createNew(QPFWPVector points){
     int pos=0,i=0;
     double totalCost=0;
     QPFWPVector newChrom,original=points;
     //qDebug()<<"The path was"<<points;
     while (i<original.size()){
        // qDebug()<<original.size();

         pos=qrand()%points.size();	//qDebug()<< pos;
         //qDebug()<<i<<points.size()<<pos;
         /// NOTE: We have to use take At to guarantee that pos gives us a value in the saiez range!
         newChrom.append(points.at(pos));
         points.remove(pos);
         if (i%2==1) { /// time to compute the distance

             totalCost+=dist(newChrom.at(i),newChrom.at(i-1));
             if (totalCost<totalCost){
                 qDebug()<<" keeping thge old tour of length"<<routeLength;
                 return original;
                }
             ///stop immidiatly if we are superior to the old one
              //qDebug()<<totalCost;
             /// fixme: previiously computed
             //originalTourCost+=dist(points.at(i),points.at(i-1));

            }
            // qDebug()<<"incrementing i";
         i++;

        }
     qDebug()<<" Total traject length"<<totalCost;

     /// we return the previous one if it is shorter
     if (routeLength>totalCost){
         routeLength=totalCost;
             //qDebug()<<" The new loops order is"<<newChrom;
         return newChrom ;
        }
     else{
             qDebug()<<" keeping thge old tour of length"<<routeLength;
         return original ;
        }
    }




Chromosome *Population::crossoverSimple(Chromosome *parent1, Chromosome *parent2){
     /**copy a first part from parent1 from 0 to pos then add all the remaining points from partne2
     at pos2 (verfinige that the point hadn't already been instrted; and complete the remaining poiunts.
     */
     int size=parent1->elements.size();
     int i=0,pos=size/2,posOther=size/2+size/3;
     qDebug()<<parent1->elements;
     qDebug()<<parent2->elements;
     Chromosome *offspring=new Chromosome(); Chromosome *referenceParent=new Chromosome();
      referenceParent->elements=parent2->elements;/// could be parent1 also
     for (i=0;i<pos;i++){
         offspring->elements.append(parent1->elements.at(i));
         referenceParent->elements.remove(referenceParent->elements.indexOf(parent1->elements.at(i)));
        }

     for(i=pos;i<size;i++){
             if (referenceParent->elements.count(parent2->elements.at(i))==0){
             ///already insterted skipping
              //qDebug()<<"skip";
            }
         else {
             offspring->elements.append(parent2->elements.at(i));
             referenceParent->elements.remove(referenceParent->elements.indexOf(parent2->elements.at(i)));
             //qDebug()<<"offs"<<offspring->elements;
             //qDebug()<<"rem"<<referenceParent->elements;
            }
        }
        for (i=0;i<referenceParent->elements.size();i++){
     offspring->elements.append(referenceParent->elements.at(i));
     }
     for (i=0;i<offspring->elements.size();i++){
     if (i%2==1){
     offspring->routeLength+=dist(offspring->elements.at(i),offspring->elements.at(i-1));
     }
     }
     qDebug()<<"new route offspring:"<<offspring->routeLength;
     qDebug()<<offspring->elements;
      //qDebug()<<"Final offspring"<<offspring->elements;
        return offspring;
    }


Chromosome::~Chromosome(){

}

void GAThread::run()
{

}





































