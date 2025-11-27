import { Card } from '@/components/ui/card';
import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { Heart, MessageCircle, Send, Bookmark, MoreHorizontal, Repeat2 } from 'lucide-react';
import { useState } from 'react';
import { toast } from 'sonner';
import { Link } from 'react-router-dom';
import type { Post, Comment } from '@/data/mockData';
import { useCurrentUser } from '@/hooks/useLocalStorage';
import { getCurrentUser } from '@/lib/localStorage';

interface PostCardProps {
  post: Post;
  onLike?: (postId: string, currentLikes: number, increment: boolean) => void;
  onComment?: (postId: string, comment: Comment) => void;
}

export const PostCard = ({ post, onLike, onComment }: PostCardProps) => {
  const { currentUser } = useCurrentUser();
  const currentUsername = getCurrentUser();
  
  // Get liked and saved posts from localStorage
  const likedPosts = JSON.parse(localStorage.getItem('likedPosts') || '{}');
  const userLikedPosts = likedPosts[currentUsername] || [];
  
  const savedPosts = JSON.parse(localStorage.getItem('savedPosts') || '{}');
  const userSavedPosts = savedPosts[currentUsername] || [];
  
  const [liked, setLiked] = useState(userLikedPosts.includes(post.id));
  const [saved, setSaved] = useState(userSavedPosts.includes(post.id));
  const [likeCount, setLikeCount] = useState(post.likes);
  const [commentText, setCommentText] = useState('');
  const [showCommentInput, setShowCommentInput] = useState(false);

  const handleLike = () => {
    const newLiked = !liked;
    setLiked(newLiked);
    setLikeCount(newLiked ? likeCount + 1 : likeCount - 1);
    onLike?.(post.id, likeCount, newLiked);
    
    // Update liked posts in localStorage
    const likedPosts = JSON.parse(localStorage.getItem('likedPosts') || '{}');
    const userLikedPosts = likedPosts[currentUsername] || [];
    
    if (newLiked) {
      likedPosts[currentUsername] = [...userLikedPosts, post.id];
    } else {
      likedPosts[currentUsername] = userLikedPosts.filter((id: string) => id !== post.id);
    }
    
    localStorage.setItem('likedPosts', JSON.stringify(likedPosts));
  };

  const handleSave = () => {
    const newSaved = !saved;
    setSaved(newSaved);
    
    // Update saved posts in localStorage
    const savedPosts = JSON.parse(localStorage.getItem('savedPosts') || '{}');
    const userSavedPosts = savedPosts[currentUsername] || [];
    
    if (newSaved) {
      savedPosts[currentUsername] = [...userSavedPosts, post.id];
      toast.success('Post saved');
    } else {
      savedPosts[currentUsername] = userSavedPosts.filter((id: string) => id !== post.id);
      toast.success('Post unsaved');
    }
    
    localStorage.setItem('savedPosts', JSON.stringify(savedPosts));
  };

  const handleRepost = () => {
    const reposts = JSON.parse(localStorage.getItem('reposts') || '{}');
    
    if (!reposts[currentUsername]) {
      reposts[currentUsername] = [];
    }
    
    if (reposts[currentUsername].includes(post.id)) {
      reposts[currentUsername] = reposts[currentUsername].filter((id: string) => id !== post.id);
      toast.success('Repost removed');
    } else {
      reposts[currentUsername].push(post.id);
      toast.success('Post reposted to your profile');
    }
    
    localStorage.setItem('reposts', JSON.stringify(reposts));
  };

  const handleAddComment = (e: React.FormEvent) => {
    e.preventDefault();
    if (!commentText.trim() || !currentUser) return;

    const newComment: Comment = {
      id: `c${Date.now()}`,
      userId: currentUser.id,
      username: currentUser.username,
      text: commentText,
      timestamp: 'Just now',
    };

    onComment?.(post.id, newComment);
    setCommentText('');
    setShowCommentInput(false);
  };

  return (
    <Card className="w-full border-border bg-card overflow-hidden" data-post-id={post.id}>
      {/* Post Header */}
      <div className="flex items-center justify-between p-3">
        <Link to={`/profile/${post.username}`} className="flex items-center gap-3 hover:opacity-80 transition-opacity">
          <Avatar className="h-9 w-9">
            <AvatarImage src={post.userAvatar} alt={post.username} />
            <AvatarFallback>{post.username[0].toUpperCase()}</AvatarFallback>
          </Avatar>
          <div>
            <p className="text-sm font-semibold text-foreground">{post.username}</p>
            {post.location && (
              <p className="text-xs text-muted-foreground">{post.location}</p>
            )}
          </div>
        </Link>
        <Button variant="ghost" size="icon" className="h-8 w-8">
          <MoreHorizontal className="h-5 w-5" />
        </Button>
      </div>

      {/* Post Image */}
      <Link to={`/post/${post.id}`}>
        <div className="relative aspect-square w-full overflow-hidden bg-muted">
          <img
            src={post.image}
            alt={post.caption}
            className="w-full h-full object-cover hover:scale-105 transition-transform duration-300"
            loading="lazy"
          />
        </div>
      </Link>

      {/* Post Actions */}
      <div className="p-3 space-y-3">
        <div className="flex items-center justify-between">
          <div className="flex items-center gap-4">
            <Button
              variant="ghost"
              size="icon"
              className="h-8 w-8 hover:opacity-70"
              onClick={handleLike}
              data-action="like"
            >
              <Heart
                className={`h-6 w-6 transition-colors ${
                  liked ? 'fill-like-red text-like-red' : 'text-foreground'
                }`}
              />
            </Button>
            <Button 
              variant="ghost" 
              size="icon" 
              className="h-8 w-8 hover:opacity-70"
              onClick={() => setShowCommentInput(!showCommentInput)}
            >
              <MessageCircle className="h-6 w-6" />
            </Button>
            <Button variant="ghost" size="icon" className="h-8 w-8 hover:opacity-70" onClick={handleRepost}>
              <Repeat2 className="h-6 w-6" />
            </Button>
            <Button variant="ghost" size="icon" className="h-8 w-8 hover:opacity-70">
              <Send className="h-6 w-6" />
            </Button>
          </div>
          <Button 
            variant="ghost" 
            size="icon" 
            className="h-8 w-8 hover:opacity-70"
            onClick={handleSave}
          >
            <Bookmark 
              className={`h-6 w-6 transition-colors ${
                saved ? 'fill-foreground text-foreground' : 'text-foreground'
              }`}
            />
          </Button>
        </div>

        {/* Likes Count */}
        <div>
          <p className="text-sm font-semibold text-foreground" data-likes-count={likeCount}>
            {likeCount.toLocaleString()} likes
          </p>
        </div>

        {/* Caption */}
        <div className="space-y-1">
          <p className="text-sm text-foreground">
            <Link to={`/profile/${post.username}`} className="font-semibold hover:opacity-70">
              {post.username}
            </Link>{' '}
            <span className="text-foreground">{post.caption}</span>
          </p>
        </div>

        {/* View Comments */}
        {post.comments.length > 0 && (
          <Link to={`/post/${post.id}`}>
            <p className="text-sm text-muted-foreground hover:text-foreground transition-colors">
              View all {post.comments.length} comments
            </p>
          </Link>
        )}

        {/* Timestamp */}
        <p className="text-xs text-muted-foreground uppercase" data-timestamp={post.timestamp}>
          {post.timestamp}
        </p>

        {/* Comment Input */}
        {showCommentInput && (
          <form onSubmit={handleAddComment} className="flex items-center gap-2 pt-2 border-t border-border">
            <Input
              placeholder="Add a comment..."
              value={commentText}
              onChange={(e) => setCommentText(e.target.value)}
              className="flex-1"
            />
            <Button type="submit" variant="ghost" size="sm" disabled={!commentText.trim()}>
              Post
            </Button>
          </form>
        )}
      </div>
    </Card>
  );
};
