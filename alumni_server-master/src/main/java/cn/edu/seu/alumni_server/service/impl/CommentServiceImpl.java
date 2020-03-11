package cn.edu.seu.alumni_server.service.impl;

import cn.edu.seu.alumni_server.controller.dto.internalRecommend.CommentDTO;
import cn.edu.seu.alumni_server.controller.dto.internalRecommend.TopCommentDTO;
import cn.edu.seu.alumni_server.dao.mapper.CommentMapper;
import cn.edu.seu.alumni_server.service.CommentService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

@Service
public class CommentServiceImpl implements CommentService {
    @Autowired
    private CommentMapper commentDao;

    @Override
    public List<TopCommentDTO> getCommentList(Long postId) {
        List<CommentDTO> commentList = commentDao.getCommentList(postId);
        HashMap<Long, TopCommentDTO> resultList = new HashMap<>();
        for(CommentDTO comment : commentList) {
            if(comment.getCommentId() == comment.getTopCommentId()) {
                resultList.put(comment.getCommentId(), new TopCommentDTO(comment));
            }
        }
        for(CommentDTO comment : commentList) {
            if(comment.getCommentId() != comment.getTopCommentId()) {
                resultList.get(comment.getTopCommentId()).getCommentList().add(comment);
            }
        }
        return new ArrayList<>(resultList.values());

    }

    @Override
    public int insertComment(CommentDTO commentDTO){
        return commentDao.insertSelective(commentDTO.toComment());
    }
}
