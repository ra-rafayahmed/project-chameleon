import { useState, useEffect } from 'react';
import type { Post, Story, StoryItem, User } from '@/data/mockData';
import * as storage from '@/lib/localStorage';

export const usePosts = () => {
  const [posts, setPosts] = useState<Post[]>([]);

  const loadPosts = () => {
    setPosts(storage.getPosts());
  };

  useEffect(() => {
    storage.initializeStorage();
    loadPosts();
  }, []);

  const addPost = (post: Post) => {
    storage.savePost(post);
    loadPosts();
  };

  const likePost = (postId: string, currentLikes: number, increment: boolean) => {
    storage.updatePost(postId, { likes: increment ? currentLikes + 1 : currentLikes - 1 });
    loadPosts();
  };

  const addComment = (postId: string, comment: any) => {
    storage.addComment(postId, comment);
    loadPosts();
  };

  const updatePost = (postId: string, updates: Partial<Post>) => {
    storage.updatePost(postId, updates);
    loadPosts();
  };

  const removePost = (postId: string) => {
    storage.deletePost(postId);
    loadPosts();
  };

  return { posts, addPost, likePost, addComment, updatePost, removePost, refreshPosts: loadPosts };

  
};

export const useStories = () => {
  const [stories, setStories] = useState<Story[]>([]);

  const loadStories = () => {
    setStories(storage.getStories());
  };

  useEffect(() => {
    storage.initializeStorage();
    loadStories();
  }, []);

  const addStory = (story: Story) => {
    storage.saveStory(story);
    loadStories();
  };

  const markAsViewed = (storyId: string) => {
    storage.markStoryAsViewed(storyId);
    loadStories();
  };

  return { stories, addStory, markAsViewed, refreshStories: loadStories };
};

export const useCurrentUser = () => {
  const [currentUser, setCurrentUser] = useState<User | undefined>();

  useEffect(() => {
    storage.initializeStorage();
    setCurrentUser(storage.getCurrentUserData());
  }, []);

  const updateProfile = (updates: Partial<User>) => {
    if (currentUser) {
      storage.updateUser(currentUser.username, updates);
      setCurrentUser(storage.getCurrentUserData());
    }
  };

  return { currentUser, updateProfile };
};
