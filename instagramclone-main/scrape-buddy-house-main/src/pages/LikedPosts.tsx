import { useState } from 'react';
import { Navigation } from '@/components/Navigation';
import { BottomNavigation } from '@/components/BottomNavigation';
import { CreatePostDialog } from '@/components/CreatePostDialog';
import { getCurrentUser } from '@/lib/localStorage';
import { usePosts } from '@/hooks/useLocalStorage';
import { Link } from 'react-router-dom';
import { Heart } from 'lucide-react';

const LikedPosts = () => {
  const currentUsername = getCurrentUser();
  const { posts, addPost } = usePosts();
  const [createPostOpen, setCreatePostOpen] = useState(false);
  
  const likedPostIds = JSON.parse(localStorage.getItem('likedPosts') || '{}')[currentUsername] || [];
  const likedPosts = posts.filter(post => likedPostIds.includes(post.id));

  return (
    <div className="min-h-screen bg-background pb-16">
      <Navigation />
      
      <main className="max-w-4xl mx-auto pt-16 pb-4 px-4">
        <h1 className="text-2xl font-semibold text-foreground mb-8">Liked Posts</h1>

        {likedPosts.length === 0 ? (
          <div className="text-center py-20">
            <Heart className="h-16 w-16 mx-auto mb-4 text-muted-foreground" />
            <p className="text-muted-foreground text-lg">No liked posts yet</p>
            <p className="text-sm text-muted-foreground mt-2">
              Posts you like will appear here
            </p>
          </div>
        ) : (
          <section className="grid grid-cols-3 gap-1" data-section="liked-posts">
            {likedPosts.map((post) => (
              <Link
                key={post.id}
                to={`/post/${post.id}`}
                className="aspect-square overflow-hidden group relative"
              >
                <img
                  src={post.image}
                  alt={post.caption}
                  className="w-full h-full object-cover group-hover:scale-105 transition-transform duration-300"
                  loading="lazy"
                />
              </Link>
            ))}
          </section>
        )}
      </main>

      <BottomNavigation onCreatePost={() => setCreatePostOpen(true)} />
      
      <CreatePostDialog
        open={createPostOpen}
        onOpenChange={setCreatePostOpen}
        onPostCreated={addPost}
      />
    </div>
  );
};

export default LikedPosts;
