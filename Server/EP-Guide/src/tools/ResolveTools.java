package tools;

import java.text.MessageFormat;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

public class ResolveTools {
	 public static Map transToMAP(Map parameterMap){
	      // 返回值Map
	      Map returnMap = new HashMap();
	      Iterator entries = parameterMap.entrySet().iterator();
	      Map.Entry entry;
	      String name = "";
	      String value = "";
	      while (entries.hasNext()) {
	          entry = (Map.Entry) entries.next();
	          name = (String) entry.getKey();
	          Object valueObj = entry.getValue();
	          if(null == valueObj){
	              value = "";
	          }else if(valueObj instanceof String[]){
	              String[] values = (String[])valueObj;
	              for(int i=0;i<values.length;i++){
	                  value = values[i] + ",";
	              }
	              value = value.substring(0, value.length()-1);
	          }else{
	              value = valueObj.toString();
	          }
	          returnMap.put(name, value);
	      }
	      return  returnMap;
	  }
	 
	 public static void getParamMap(HttpServletRequest request) {
		 Map<String, String[]> paramMap = request.getParameterMap();
			for(Map.Entry<String, String[]> entry :paramMap.entrySet()){
	            String paramName = entry.getKey();
	            String paramValue = "";
	            String[] paramValueArr = entry.getValue();
	            for (int i = 0; paramValueArr!=null && i < paramValueArr.length; i++) {
	                if (i == paramValueArr.length-1) {
	                    paramValue+=paramValueArr[i];
	                }else {
	                    paramValue+=paramValueArr[i]+",";
	                }
	            }
	            System.out.println("getIN:!");
	            System.out.println(MessageFormat.format("{0}={1}", paramName,paramValue));
	        }
	}
}
