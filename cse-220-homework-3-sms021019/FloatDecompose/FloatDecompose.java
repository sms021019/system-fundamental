public class FloatDecompose {
   public static int getSign(float value){
       if(value < 0) {
           return 1;
       }else{
           return 0;
       }
   }
   public static int getExponent(float value){
       String temp = floatTobits(value);
       String ans = temp.substring(1,9);
       return Integer.parseInt(ans,2);

   }

   public static int getFraction(float value){
       String temp = floatTobits(value);
       String ans = temp.substring(9);
       return Integer.parseInt(ans,2);
   }

   public static String floatTobits (Float val){
       int ans = Float.floatToIntBits(val);
       String temp = String.format("%32s", Integer.toBinaryString(ans)).replace(' ', '0');
       return temp;
   }

   public static float makeFloat(int sign, int exp, int frac){
       int ans;
       ans = sign << 8;
       ans = ans | exp;
       ans = ans << 23;
       ans = ans | frac;
       float jjinans = Float.intBitsToFloat(ans);
       return jjinans;

   }
}


