package cn.edu.seu.alumni_server.service;

import cn.edu.seu.alumni_server.controller.dto.internalRecommend.CommentDTO;
import cn.edu.seu.alumni_server.controller.dto.internalRecommend.TopCommentDTO;

import java.util.List;

public interface CommentService {

    List<TopCommentDTO> getCommentList(Long postId);

    int insertComment(CommentDTO commentDTO);
}
