#include <cmath>
#include <iostream>

#include "CSimulator.h"

int main(int argc, char * argv[])
{

      // dummy vector. Replace in your code with the actual circuit vector.
      int vec1[] = {0, 1, 3, 3, 2, 2, 0, 4, 1, 1, 1, 0, 5}  ;

      // dummy vector. Replace in your code with the actual circuit vector.

      int vec2[] = {0, 1, 3, 2, 4, 4, 3, 1, 3, 6, 1, 1, 0, 5, 1, 1 }  ;

      // Test value based on stub Evaluate_Circuit function.
      // Replace with actual performance value.

      std::cout << "Evaluate_Circuit(13, vec1) close to 110.25 :\n";
      double result = Evaluate_Circuit(13, vec1);
      std::cout << "Evaluate_Circuit(13, vec1) = "<< result <<"\n";

      if (std::fabs(result - 110.25)<0.01) {
                  std::cout << "pass\n";
            } else {
	        std::cout << "fail\n";
              return 1;
           }

      // Test value based on stub Evaluate_Circuit function.
      // Replace with actual performance value.
      
      std::cout << "Evaluate_Circuit(16, vec2) close to 167.38:\n";
      result =  Evaluate_Circuit(16, vec2);
      std::cout << "Evaluate_Circuit(16, vec2) = "<< result<<"\n";
      if (std::fabs(result - 167.38)<0.01)
	        std::cout << "pass\n";
      else
            { 
	        std::cout << "fail";
              return 1;
            }
	
}
