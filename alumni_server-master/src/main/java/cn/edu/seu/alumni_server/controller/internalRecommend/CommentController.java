package cn.edu.seu.alumni_server.controller.internalRecommend;

import cn.edu.seu.alumni_server.common.CONST;
import cn.edu.seu.alumni_server.common.Utils;
import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.common.token.Acl;
import cn.edu.seu.alumni_server.controller.dto.internalRecommend.CommentDTO;
import cn.edu.seu.alumni_server.controller.dto.internalRecommend.TopCommentDTO;
import cn.edu.seu.alumni_server.service.CommentService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import javax.servlet.http.HttpServletRequest;
import java.util.List;

@RestController
@RequestMapping("/comment")
public class CommentController {

    @Autowired
    private CommentService commentService;

    @Autowired
    HttpServletRequest request;

    /**
     * 获取某条帖子下的评论列表
     * */
    @GetMapping("/list")
    public WebResponse getCommentList(@RequestParam long postId) {
        List<TopCommentDTO> list = commentService.getCommentList(postId);
        return new WebResponse().success(list);
    }

    /**
     * 新增评论
     * */
    @PostMapping("/")
    public WebResponse newComment(@RequestBody CommentDTO commentDTO) {
        Long accountId = (Long) request.getAttribute(CONST.ACL_ACCOUNTID);

        commentDTO.setAccountId(accountId);
        commentDTO.setCommentId(Utils.generateId());
        commentService.insertComment(commentDTO);
        return new WebResponse().success();
    }
}
