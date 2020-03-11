package cn.edu.seu.alumni_server.controller;

import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.common.token.Acl;
import cn.edu.seu.alumni_server.dao.entity.MyCollection;
import cn.edu.seu.alumni_server.dao.mapper.MyCollectionMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import tk.mybatis.mapper.entity.Example;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;

@RestController
//@Acl
public class MyCollectionController {

    @Autowired
    HttpServletRequest request;

    @Autowired
    MyCollectionMapper myCollectionMapper;

    @PostMapping("/myCollection")
    public WebResponse changeMyCollectionStatus(@RequestBody Map req){
        Long accountId = (Long) request.getAttribute("accountId");
        Long resourceId = (Long) req.get("resourceId");
        Integer type = (Integer)req.get("type");
        Boolean status = (Boolean)req.get("status");

        MyCollection myCollection = new MyCollection();
        myCollection.setAccountId(accountId);
        myCollection.setResourceId(resourceId);

        if(myCollectionMapper.select(myCollection).size() == 0) {
            myCollection.setType(type);
            myCollection.setStatus(status);
            myCollectionMapper.insertSelective(myCollection);
        }
        else{
            Example example = new Example(MyCollection.class);
            Example.Criteria criteria = example.createCriteria();
            criteria.andEqualTo("accountId", accountId);
            criteria.andEqualTo("resourceId", resourceId);

            myCollection.setType(type);
            myCollection.setStatus(status);

            myCollectionMapper.updateByExampleSelective(myCollection, example);
        }
        return new WebResponse();
    }




}
