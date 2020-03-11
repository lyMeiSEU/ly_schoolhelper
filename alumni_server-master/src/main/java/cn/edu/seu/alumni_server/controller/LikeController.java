package cn.edu.seu.alumni_server.controller;

import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.dao.entity.Like;
import cn.edu.seu.alumni_server.dao.mapper.LikeMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import tk.mybatis.mapper.entity.Example;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;

@RestController
public class LikeController {

    @Autowired
    private LikeMapper likeMapper;

    @Autowired
    HttpServletRequest request;

    @PostMapping("/like")
    public WebResponse changeLikeStatus(@RequestBody Map req){
        Long accountId = (Long) request.getAttribute("accountId");
        Long resourceId = (Long) req.get("resourceId");
        Integer type = (Integer)req.get("type");
        Boolean status = (Boolean)req.get("status");

        Like like = new Like();
        like.setAccountId(accountId);
        like.setResourceId(resourceId);
        like.setStatus(status);
        like.setType(type);

        if(likeMapper.queryLikeList(like).size() == 0) {
            likeMapper.insertLike(like);
        }
        else {
            likeMapper.updateLike(like);
        }
        return new WebResponse();
    }


}
