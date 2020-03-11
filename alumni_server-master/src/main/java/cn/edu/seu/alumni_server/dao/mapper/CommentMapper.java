package cn.edu.seu.alumni_server.dao.mapper;

import cn.edu.seu.alumni_server.controller.dto.internalRecommend.CommentDTO;
import cn.edu.seu.alumni_server.dao.entity.Comment;
import org.springframework.stereotype.Repository;
import tk.mybatis.mapper.common.Mapper;

import java.util.List;

@Repository
public interface CommentMapper extends Mapper<Comment> {

    List<CommentDTO> getCommentList(Long postId);
}
