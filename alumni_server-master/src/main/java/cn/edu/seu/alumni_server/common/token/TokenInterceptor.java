package cn.edu.seu.alumni_server.common.token;

import cn.edu.seu.alumni_server.common.CONST;
import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.common.dto.WebResponseEnum;
import com.google.gson.Gson;
import org.springframework.web.method.HandlerMethod;
import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.PrintWriter;

/**
 * token拦截器，做token校验用
 *
 */
public class TokenInterceptor extends HandlerInterceptorAdapter {
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        Acl acl = this.queryRequestAcl(request, handler);
        if (null == acl) {
            return true;
        } else {
            return checkToken(request, response, acl);
        }
    }

    Acl queryRequestAcl(HttpServletRequest request, Object handler) {
        HandlerMethod handlerMethod = (HandlerMethod) handler;
        Acl acl = (Acl) handlerMethod.getMethod().getAnnotation(Acl.class);
        if (acl == null) {
            acl = (Acl) handlerMethod.getBeanType().getAnnotation(Acl.class);
        }
        return acl;
    }

    Boolean checkToken(HttpServletRequest request, HttpServletResponse response, Acl acl) throws Exception {
        String token = request.getHeader(TokenUtil.TOKEN_HEADER);

        if (!TokenUtil.checkToken(token)) {
            response.reset();

            response.setCharacterEncoding("UTF-8");
            response.setContentType("text/plain;charset=UTF-8");

            PrintWriter pw = response.getWriter();

            pw.write(new Gson().toJson(
                    new WebResponse(WebResponseEnum.tokenError)
            ));

            pw.flush();
            pw.close();
//            response.sendError(401);
            return false;
        } else {
            request.setAttribute(CONST.ACL_ACCOUNTID, TokenUtil.getAccountId(token));
        }
        return true;
    }

}